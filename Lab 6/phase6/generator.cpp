/*
 * File:	generator.cpp
 *
 * Description:	This file contains the public and member function
 *		definitions for the code generator for Simple C.
 *
 *		Extra functionality:
 *		- putting all the global declarations at the end
 */

# include <cassert>
# include <iostream>
# include <unordered_map>
# include "generator.h"

using namespace std;

static int offset;
static string funcname;
static ostream &operator <<(ostream &ostr, Expression *expr);

static Register *eax = new Register("%eax", "%al");
static Register *ecx = new Register("%ecx", "%cl");
static Register *edx = new Register("%edx", "%dl");

static vector<Register *> registers = {eax, ecx, edx};

static unordered_map<string, Label> str_labels;


/*
 * Function:	align (private)
 *
 * Description:	Return the number of bytes necessary to align the given
 *		offset on the stack.
 */

static int align(int offset)
{
    if (offset % STACK_ALIGNMENT == 0)
	return 0;

    return STACK_ALIGNMENT - (abs(offset) % STACK_ALIGNMENT);
}


/*
 * Function:	operator << (private)
 *
 * Description:	Convenience function for writing the operand of an
 *		expression using the output stream operator.
 */

static ostream &operator <<(ostream &ostr, Expression *expr)
{
    if (expr->_register != nullptr)
	return ostr << expr->_register;

    expr->operand(ostr);
    return ostr;
}


/*
 * Function:	Expression::operand
 *
 * Description:	Write an expression as an operand to the specified stream.
 */

void Expression::operand(ostream &ostr) const
{
  //ut << "#expression operand" << endl;
    assert(_offset != 0);
    ostr << _offset << "(%ebp)";
}


/*
 * Function:	Identifier::operand
 *
 * Description:	Write an identifier as an operand to the specified stream.
 */

void Identifier::operand(ostream &ostr) const
{
  //ut << "#identifier operand" << endl;
    if (_symbol->_offset == 0)
	ostr << global_prefix << _symbol->name();
    else
	ostr << _symbol->_offset << "(%ebp)";
}


/*
 * Function:	Number::operand
 *
 * Description:	Write a number as an operand to the specified stream.
 */

void Number::operand(ostream &ostr) const
{
    ostr << "$" << _value;
}

void String::operand(ostream &ostr) const {
    Label str_label;
    if (str_labels.find(_value) == str_labels.end()) { // string is not in global map
        str_labels.insert({_value, str_label});
    }
    else {
        str_label = str_labels.find(_value)->second;
    }
    ostr << str_label;
}


/*
 * Function:	Call::generate
 *
 * Description:	Generate code for a function call expression.
 *
 * 		On a 32-bit Linux platform, the stack needs to be aligned
 * 		on a 4-byte boundary.  (Online documentation seems to
 * 		suggest that 16 bytes are required, but 4 bytes seems to
 * 		work and is much easier.)  Since all arguments are 4-bytes
 *		wide, the stack will always be aligned.
 *
 *		On a 32-bit OS X platform, the stack needs to aligned on a
 *		16-byte boundary.  So, if the stack will not be aligned
 *		after pushing the arguments, we first adjust the stack
 *		pointer.  However, this trick only works if none of the
 *		arguments are themselves function calls.
 *
 *		To handle nested function calls, we need to generate code
 *		for the nested calls first, which requires us to save their
 *		results and then push them on the stack later.  For
 *		efficiency, we only first generate code for the nested
 *		calls, but generate code for ordinary arguments in place.
 */

void Call::generate()
{
    unsigned numBytes;


    /* Generate code for any nested function calls first. */

    numBytes = 0;
    cout << "#function call:" << endl;

    for (int i = _args.size() - 1; i >= 0; i --) {
	numBytes += _args[i]->type().size();

	if (STACK_ALIGNMENT != SIZEOF_ARG && _args[i]->_hasCall)
	    _args[i]->generate();
    }

    /*for (unsigned i = 0; i < registers.size(); i++) {
      load(nullptr, registers[i]);
      }*/


    /* Align the stack if necessary. */

    if (align(numBytes) != 0) {
	cout << "\tsubl\t$" << align(numBytes) << ", %esp" << endl;
	numBytes += align(numBytes);
    }


    /* Generate code for any remaining arguments and push them on the stack. */

    for (int i = _args.size() - 1; i >= 0; i --) {
	if (STACK_ALIGNMENT == SIZEOF_ARG || !_args[i]->_hasCall)
	    _args[i]->generate();

	cout << "\tpushl\t" << _args[i] << endl;
	assign(_args[i], nullptr);
    }


    /* Call the function and then reclaim the stack space. */

    load(nullptr, eax);
    load(nullptr, ecx);
    load(nullptr, edx);

    cout << "\tcall\t" << global_prefix << _id->name() << endl;

    if (numBytes > 0)
	cout << "\taddl\t$" << numBytes << ", %esp" << endl;

    assign(this, eax);
}


/*
 * Function:	Block::generate
 *
 * Description:	Generate code for this block, which simply means we
 *		generate code for each statement within the block.
 */

void Block::generate()
{
    cout << "#block:" << endl;
    for (auto stmt : _stmts) {
        stmt->generate();

        for (auto reg : registers) {
	  assert(reg->_node == nullptr);
	}
    }
}


/*
 * Function:	Simple::generate
 *
 * Description:	Generate code for a simple (expression) statement, which
 *		means simply generating code for the expression.
 */

void Simple::generate()
{
    _expr->generate();
    assign(_expr, nullptr);
}


/*
 * Function:	Function::generate
 *
 * Description:	Generate code for this function, which entails allocating
 *		space for local variables, then emitting our prologue, the
 *		body of the function, and the epilogue.
 */

void Function::generate()
{
    int param_offset;


    /* Assign offsets to the parameters and local variables. */

    param_offset = 2 * SIZEOF_REG;
    offset = param_offset;
    allocate(offset);


    /* Generate our prologue. */

    cout << "#function prologue:" << endl;
    funcname = _id->name();
    cout << global_prefix << funcname << ":" << endl;
    cout << "\tpushl\t%ebp" << endl;
    cout << "\tmovl\t%esp, %ebp" << endl;
    cout << "\tsubl\t$" << funcname << ".size, %esp" << endl;


    /* Generate the body of this function. */

    _body->generate();


    /* Generate our epilogue. */

    cout << "#function epilogue:" << endl;
    cout << endl << global_prefix << funcname << ".exit:" << endl;
    cout << "\tmovl\t%ebp, %esp" << endl;
    cout << "\tpopl\t%ebp" << endl;
    cout << "\tret" << endl << endl;

    offset -= align(offset - param_offset);
    cout << "\t.set\t" << funcname << ".size, " << -offset << endl;
    cout << "\t.globl\t" << global_prefix << funcname << endl << endl;
}


/*
 * Function:	generateGlobals
 *
 * Description:	Generate code for any global variable declarations.
 */

void generateGlobals(Scope *scope)
{
    const Symbols &symbols = scope->symbols();

    for (auto symbol : symbols)
	if (!symbol->type().isFunction()) {
	    cout << "\t.comm\t" << global_prefix << symbol->name() << ", ";
	    cout << symbol->type().size() << endl;
	}

    cout << "\t.data" << endl;
    for (pair<string, Label> str_pair : str_labels) {
        cout << str_pair.second << ":\t.asciz\t\"" << str_pair.first << "\"" << endl;
    }
}


/*
 * Function:	Assignment::generate
 *
 * Description:	Generate code for an assignment statement.
 *
 *		NOT FINISHED: Only works if the right-hand side is an
 *		integer literal and the left-hand side is an integer
 *		scalar.
 */

void Assignment::generate()
{
  //assert(dynamic_cast<Number *>(_right));
  //assert(dynamic_cast<Identifier *>(_left));

    cout << "#assignment:" << endl;
    Expression *pointer;
    _right->generate();
    if (_left->isDereference(pointer)) {
        pointer->generate();
        if (pointer->_register == nullptr) {
            load(pointer, getreg());
        }
        if (_right->_register == nullptr) {
            load(_right, getreg());
        }
        if (_right->type().size() == 1) {
	  cout << "\tmovzbl\t" << _right << ", (" << pointer << ")" << endl;
        }
        else {
	  cout << "\tmovl\t" << _right << ", (" << pointer << ")" << endl;
        }
        assign(pointer, nullptr);
    }
    else {
        if (_right->_register == nullptr) {
            load(_right, getreg());
        }
        _left->generate();
        if (_right->type().size() == 1) {
            cout << "\tmovzbl\t" << _right << ", " << _left << endl;
        }
        else {
            cout << "\tmovl\t" << _right << ", " << _left << endl;
        }
        //assign(_right, nullptr);
    }
    assign(_right, nullptr);
}

void load(Expression *expr, Register *reg) {
    if (reg->_node != expr) {
        if (reg->_node != nullptr) {
            offset -= reg->_node->type().size();
            reg->_node->_offset = offset;
            cout << "\tmovl\t" << reg << ", ";
            cout << offset << "(%ebp)" << endl;
        }
        if (expr != nullptr) {
            cout << (expr->type().size() == 1 ? "\tmovsbl\t" : "\tmovl\t");
            cout << expr << ", " << reg << endl;
        }
        assign(expr, reg);
    }
}

void assign(Expression *expr, Register *reg) {
    if (expr != nullptr) {
        if (expr ->_register != nullptr) {
            expr->_register->_node = nullptr;
        }
        expr->_register = reg;
    }
    if (reg != nullptr) {
        if (reg->_node != nullptr) {
            reg->_node->_register = nullptr;
        }
        reg->_node = expr;
    }
}

Register *getreg() {
    for (auto reg : registers) {
        if (reg->_node == nullptr) {
            return reg;
        }
    }
    load(nullptr, registers[0]);
    return registers[0];
}

void Dereference::generate() {
  cout << "#dereference" << endl;
    _expr->generate();
    if (_expr->_register == nullptr) {
        load(_expr, getreg());
    }
    if (_expr->type().size() == 1) {
        cout << "\tmovzbl\t" << "(" << _expr << "), " << _expr << endl;
    }
    else {
      cout << "\tmovl\t" << "(" << _expr << "), " << _expr << endl;
    }
    assign(this, _expr->_register);
}

void Address::generate() {
  cout << "#address" << endl;
    Expression *pointer;
    if (_expr->isDereference(pointer)) {
        pointer->generate();
        if (pointer->_register == nullptr) {
            load(pointer, getreg());
        }
        assign(this, pointer->_register);
    }
    else {
        assign(this, getreg());
        cout << "\tleal\t" << _expr << ", " << this << endl;
    }
}

void Cast::generate() {
    _expr->generate();
    if (_expr->_register == nullptr) {
        load(_expr, getreg());
    }
    assign(this, _expr->_register);
}

void Not::generate() {
    _expr->generate();
    if (_expr->_register == nullptr) {
        load(_expr, getreg());
    }
    cout << "\tcmpl\t" << "$0, " << _expr << endl;
    cout << "\tsete\t" << _expr->_register->byte() << endl;
    cout << "\tmovzbl\t" << _expr->_register->byte() << ", " << _expr << endl;

    assign(this, _expr->_register);
}

void Negate::generate() {
    _expr->generate();
    if (_expr->_register == nullptr) {
        load(_expr, getreg());
    }
    cout << "\tnegl\t" << _expr << endl;

    assign(this, _expr->_register);
}

void Divide::generate() {
    _left->generate();
    _right->generate();

    load(_left, eax);
    load(_right, ecx);
    load(nullptr, edx);

    cout << "\tcltd\t" << endl;
    cout << "\tidivl\t" << "%ecx" << endl;

    assign(nullptr, eax);
    assign(nullptr, ecx);
    assign(nullptr, edx);
    assign(this, eax);
}

void Remainder::generate() {
    _left->generate();
    _right->generate();

    load(_left, eax);
    load(_right, ecx);
    load(nullptr, edx);

    cout << "\tcltd\t" << endl;
    cout << "\tidivl\t" << "%ecx" << endl;

    assign(nullptr, eax);
    assign(nullptr, ecx);
    assign(nullptr, edx);
    assign(this, edx);
}

void compute(Expression *result, Expression *left, Expression *right, const string &opcode) {
    left->generate();
    right->generate();

    if (left->_register == nullptr) {
        load(left, getreg());
    }
    cout << "\t" << opcode << "\t" << right << ", " << left << endl;

    assign(right, nullptr);
    assign(result, left->_register);
}

void Add::generate() {
    compute(this, _left, _right, "addl");
}

void Subtract::generate() {
    compute(this, _left, _right, "subl");
}

void Multiply::generate() {
    compute(this, _left, _right, "imull");
}

void compare(Expression *result, Expression *left, Expression *right, const string &opcode) {
    left->generate();
    right->generate();

    if (left->_register == nullptr) {
        load(left, getreg());
    }
    cout << "\tcmpl\t" << right << ", " << left << endl;
    cout << "\t" << opcode << "\t" << left->_register->byte() << endl;
    cout << "\tmovzbl\t" << left->_register->byte() << ", " << left->_register << endl;

    assign(result, left->_register);
}

void LessThan::generate() {
    compare(this, _left, _right, "setl");
}

void GreaterThan::generate() {
    compare(this, _left, _right, "setg");
}

void LessOrEqual::generate() {
    compare(this, _left, _right, "setle");
}

void GreaterOrEqual::generate() {
    compare(this, _left, _right, "setge");
}

void Equal::generate() {
    compare(this, _left, _right, "sete");
}

void NotEqual::generate() {
  cout << "#not equal" << endl;
    compare(this, _left, _right, "setne");
}

void LogicalAnd::generate() {
  cout << "#logical and" << endl;
  /*_left->generate();
    if (_left->_register == nullptr) {
      load(_left, getreg());
      }*/

    Label skip;
    _left->test(skip, false);

    /*cout << "\tcmpl\t$0, " << _left << endl;
      cout << "\tjne\t" << skip << endl;*/

    _right->generate();
    if (_right->_register == nullptr) {
      load(_right, getreg());
    }
    cout << "\tcmpl\t$0, " << _right << endl;

    cout << skip << ":" << endl;
    cout << "\tsetne\t" << _right->_register->byte() << endl;
    cout << "\tmovzbl\t" << _right->_register->byte() << ", " << _right << endl;
    assign(this, _right->_register);
    //cout << "\tmovl\t" << _right << ", " << this << endl;
}

void LogicalOr::generate() {
  cout << "#logical or" << endl;
  /*_left->generate();
    if (_left->_register == nullptr) {
      load(_left, getreg());
      }*/

    Label skip;
    _left->test(skip, true);

    /*cout << "\tcmpl\t$0, " << _left << endl;
      cout << "\tjne\t" << skip << endl;*/

    _right->generate();
    if (_right->_register == nullptr) {
      load(_right, getreg());
    }
    cout << "\tcmpl\t$0, " << _right << endl;

    cout << skip << ":" << endl;
    cout << "\tsetne\t" << _right->_register->byte() << endl;
    cout << "\tmovzbl\t" << _right->_register->byte() << ", " << _right << endl;
    assign(this, _right->_register);
    //cout << "\tmovl\t" << _right << ", " << this << endl;
}

void Expression::test(const Label &label, bool ifTrue) {
    generate();

    if (_register == nullptr) {
        load(this, getreg());
    }
    cout << "\tcmpl\t$0, " << this << endl;
    cout << (ifTrue ? "\tjne\t" : "\tje\t") << label << endl;

    assign(this, nullptr);
}

void Return::generate() {
  cout << "#return statement" << endl;
    _expr->generate();
    load(_expr, eax);
    cout << "\tjmp\t" << funcname << ".exit" << endl;
    assign(_expr, nullptr);
}

void While::generate() {
  cout << "#while loop" << endl;
    Label loop, exit;

    cout << loop << ":" << endl;

    _expr->test(exit, false);
    _stmt->generate();

    cout << "\tjmp\t" << loop << endl;
    cout << exit << ":" << endl;
}

void For::generate() {
  cout << "#for loop" << endl;
    Label loop, exit;
    _init->generate();
    cout << loop << ":" << endl;

    _expr->test(exit, false);
    _stmt->generate();
    _incr->generate();

    cout << "\tjmp\t" << loop << endl;
    cout << exit << ":" << endl;
}

void If::generate() {
  cout << "#if statement" << endl;
    Label skip, exit;

    _expr->test(skip, false);
    _thenStmt->generate();

    if (_elseStmt != nullptr) {
      cout << "#else statement" << endl;
      cout << "\tjmp\t" << exit << endl;
      cout << skip << ":" << endl;
      _elseStmt->generate();
      cout << exit << ":" << endl;
    }
    else {
      cout << skip << ":" << endl;
    }
    //cout << exit << ":" << endl;
}
