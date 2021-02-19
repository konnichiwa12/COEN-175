# include <iostream>

# include "checker.h"
# include "lexer.h"
# include "scope.h"
# include "symbol.h"
# include "tokens.h"

using namespace std;

static string E1 = "redefinition of '%s'";
static string E2 = "conflicting types for '%s'";
static string E3 = "redeclaration of '%s'";
static string E4 = "'%s' undeclared";
static string E5 = "'%s' has type void";

static Scope* globalScope = nullptr;
static Scope* currentScope = nullptr;

void openScope() {
    cout << "opening scope" << endl;
    currentScope = new Scope(currentScope);
    if (globalScope == nullptr) {
        globalScope = currentScope;
    }
}

void closeScope() {
    cout << "closing scope" << endl;
    currentScope = currentScope->enclosing();
}

void declareVariable(const string &name, const Type &type){
    cout << "declare " << name << " as (" << type.specifier() << ", " << type.indirection() << ", SCALAR)" << endl;

    Symbol *symbol = currentScope->find(name);

    if (symbol == nullptr) {
        if (type.specifier() == VOID && type.indirection() == 0) {
            report(E5, name);
            return;
        }
        symbol = new Symbol(name, type);
        currentScope->insert(symbol);
    }
    else if (currentScope->enclosing() != nullptr) { // symbol exists at this point
        report(E3, name);
    }
    else if (symbol->type() != type){
        report(E2, name);
    }
}

void declareArray(const string &name, const Type &type){
    // cout << "Length: " << type.length() << endl;
    cout << "declare " << name << " as (" << type.specifier() << ", " << type.indirection() << ", ARRAY)" << endl;

    Symbol *symbol = currentScope->find(name);

    if (symbol == nullptr) {
        if (type.specifier() == VOID && type.indirection() == 0) {
            report(E5, name);
            return;
        }
        symbol = new Symbol(name, type);
        currentScope->insert(symbol);
    }
    else if (currentScope->enclosing() != nullptr) { // symbol exists at this point
        report(E3, name);
    }
    else if (symbol->type() != type){
        report(E2, name);
    }
}

void declareFunction(const string &name, const Type &type){
    // cout << "Parameters: " << type.parameters() << endl;
    cout << "declare " << name << " as (" << type.specifier() << ", " << type.indirection() << ", FUNCTION)" << endl;

    Symbol *symbol = globalScope->find(name);

    if (symbol == nullptr) {
        symbol = new Symbol(name, type);
        globalScope->insert(symbol);
    }
    else if (symbol->type() != type){
        report(E2, name);
    }
    else if (symbol->_defined) {
        report(E1, name);
    }
}

void defineFunction(const string &name, const Type &type){
    cout << "define function " << name << endl;
    // current->enclosing()->insert(...);
    Symbol *symbol = globalScope->find(name);

    if (symbol == nullptr) {
        symbol = globalScope->lookup(name); // define it and place at the beginning of linked list
        symbol = new Symbol(name, type, true);
        globalScope->insert(symbol);
    }
    else if (symbol->type() != type) {
        report(E2, name);
    }
    else if (symbol->_defined) {
        report(E1, name);
    }
}

void checkID(const string &id, const Type &type){
    cout << "check " << id << endl;

    Symbol *symbol = currentScope->find(id);

    if (symbol == nullptr) {
        // now check for outer scopes if it's not in inner scope
        symbol = currentScope->lookup(id);
        if (symbol == nullptr) {
            /*if (type.kind() == 2) { // check if the ID corresponds to FUNCTION
                declareFunction(id, type);
            }
            else {*/
                report(E4, id);
            //}
        }
    }

}