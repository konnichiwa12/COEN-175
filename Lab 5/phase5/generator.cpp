# include "generator.h"
# include <iostream>

using namespace std;

static ostream &operator <<(ostream &ostr, Expression *expr)
{
    // node->write(ostr);
    expr->operand(ostr);
    return ostr;
}

void generateGlobals(const Symbols &global_vars) {
    cout << "#global variables:" << endl;
    for (unsigned i = 0; i < global_vars.size(); i++) {
        cout << "\t.comm\t" << global_vars[i]->name() << ", " << global_vars[i]->type().size() << endl;
    }
}

void Assignment::generate() {
    cout << "#assignment:" << endl;
    _right->generate();
    _left->generate();
    cout << "\tmovl\t" << _right << ", %eax" << endl;
    cout << "\tmovl\t%eax, " << _left << endl;
}

void Block::generate() {
    cout << "#block:" << endl;
    for (auto stmt : _stmts) {
        stmt->generate();
    }
}

void Call::generate() {
    cout << "#function call:" << endl;
    for (int i = _args.size()-1; i >= 0; i--) { // push args from right to left first
        _args[i]->generate();
        cout << "\tpushl\t" << _args[i] << endl;
    }
    cout << "\tcall\t" << _id->name() << endl;
}

void Function::generate() {
    // assign offsets
    int params_offset = 8;
    int local_offset = 0;
    Symbols variables = _body->declarations()->symbols();
    unsigned num_params = _id->type().parameters()->size();

    for (unsigned i = 0; i < variables.size(); ++i) {
        if (i < num_params) {
            variables[i]->_offset = params_offset;
            params_offset += variables[i]->type().size();
        }
        else {
            local_offset -= variables[i]->type().size();
            variables[i]->_offset = local_offset;
        }
    }

    cerr << "Params offset: " << params_offset << endl;
    cerr << "Locals offset: " << local_offset << endl;

    // generate prologue
    cout << _id->name() << ":" << endl;
    cout << "#function prologue:" << endl;
    cout << "\tpushl\t%ebp" << endl;
    cout << "\tmovl\t%esp, %ebp" << endl;
    cout << "\tsubl\t$" << -local_offset << ", %esp" << endl;

    // generate code for body of function
    _body->generate();

    // generate epilogue
    cout << "#function epilogue:" << endl;
    cout << "\tmovl\t%ebp, %esp" << endl;
    cout << "\tpopl\t%ebp" << endl;
    cout << "\tret" << endl;
    cout << "\t.globl\t" << _id->name() << endl;
}

void Simple::generate() {
    _expr->generate();
}

void Expression::operand(ostream &ostr) const {

}

void Number::operand(ostream &ostr) const {
    ostr << "$" << _value;
}

void Identifier::operand(ostream &ostr) const {
    if (_symbol->_offset == 0) { // if it's a global, refer to it by name
        cout << _symbol->name();
    }
    else { // otherwise, use its offset from the base pointer
        cout << _symbol->_offset << "(%ebp)";
    }
}