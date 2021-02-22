/*
 * File:	checker.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the semantic checker for Simple C.
 *
 *		If a symbol is redeclared, the redeclaration is discarded
 *		and the original declaration is retained.
 *
 *		Extra functionality:
 *		- inserting an undeclared symbol with the error type
 */

# include <iostream>
# include "lexer.h"
# include "checker.h"
# include "tokens.h"
# include "Symbol.h"
# include "Scope.h"
# include "Type.h"


using namespace std;

static Scope *outermost, *toplevel;
static const Type error, integer(INT);

static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string undeclared = "'%s' undeclared";
static string void_object = "'%s' has type void";

static string E1 = "invalid return type";
static string E2 = "invalid type for test expression";
static string E3 = "lvalue required in expression";
static string E4 = "invalid operands to binary %s";
static string E5 = "invalid operand to unary %s";
static string E6 = "called object is not a function";
static string E7 = "invalid arguments to called function";


/*
 * Function:	openScope
 *
 * Description:	Create a scope and make it the new top-level scope.
 */

Scope *openScope()
{
    toplevel = new Scope(toplevel);

    if (outermost == nullptr)
	outermost = toplevel;

    return toplevel;
}


/*
 * Function:	closeScope
 *
 * Description:	Remove the top-level scope, and make its enclosing scope
 *		the new top-level scope.
 */

Scope *closeScope()
{
    Scope *old = toplevel;
    toplevel = toplevel->enclosing();
    return old;
}


/*
 * Function:	defineFunction
 *
 * Description:	Define a function with the specified NAME and TYPE.  A
 *		function is always defined in the outermost scope.  This
 *		definition always replaces any previous definition or
 *		declaration.
 */

Symbol *defineFunction(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = outermost->find(name);

    if (symbol != nullptr) {
	if (symbol->type().isFunction() && symbol->type().parameters()) {
	    report(redefined, name);
	    delete symbol->type().parameters();

	} else if (type != symbol->type())
	    report(conflicting, name);

	outermost->remove(name);
	delete symbol;
    }

    symbol = new Symbol(name, type);
    outermost->insert(symbol);
    return symbol;
}


/*
 * Function:	declareFunction
 *
 * Description:	Declare a function with the specified NAME and TYPE.  A
 *		function is always declared in the outermost scope.  Any
 *		redeclaration is discarded.
 */

Symbol *declareFunction(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = outermost->find(name);

    if (symbol == nullptr) {
	symbol = new Symbol(name, type);
	outermost->insert(symbol);

    } else if (type != symbol->type()) {
	report(conflicting, name);
	delete type.parameters();

    } else
	delete type.parameters();

    return symbol;
}


/*
 * Function:	declareVariable
 *
 * Description:	Declare a variable with the specified NAME and TYPE.  Any
 *		redeclaration is discarded.
 */

Symbol *declareVariable(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = toplevel->find(name);

    if (symbol == nullptr) {
	if (type.specifier() == VOID && type.indirection() == 0)
	    report(void_object, name);

	symbol = new Symbol(name, type);
	toplevel->insert(symbol);

    } else if (outermost != toplevel)
	report(redeclared, name);

    else if (type != symbol->type())
	report(conflicting, name);

    return symbol;
}


/*
 * Function:	checkIdentifier
 *
 * Description:	Check if NAME is declared.  If it is undeclared, then
 *		declare it as having the error type in order to eliminate
 *		future error messages.
 */

Symbol *checkIdentifier(const string &name)
{
    Symbol *symbol = toplevel->lookup(name);

    if (symbol == nullptr) {
        report(undeclared, name);
        symbol = new Symbol(name, error);
        toplevel->insert(symbol);
    }

    return symbol;
}

Type checkLogical(const Type &left, const Type &right, const string &op) { // this is the exact same for checkLogicalAnd
    if (left == error || right == error) {
         return error;
    }
    Type promote_left = left.promote();
    Type promote_right = right.promote();
    if (promote_left.isValue() && promote_right.isValue()) {
        return integer;
    }
    report(E4, op);
    return error;
}

Type checkLogicalOr(const Type &left, const Type &right) {
    return checkLogical(left, right, "||");
}

Type checkLogicalAnd(const Type &left, const Type &right) {
     return checkLogical(left, right, "&&");
}

Type checkEquality(const Type &left, const Type &right) {
    if (left == error || right == error) {
        return error;
    }
    if (left.isCompatibleWith(right)) {
        return integer;
    }
    report(E4, "==");
    return error;
}

Type checkNotEquality(const Type &left, const Type &right) {
    if (left == error || right == error) {
        return error;
    }
    if (left.isCompatibleWith(right)) {
        return integer;
    }
    report(E4, "!=");
    return error;
}

Type checkRelationalExpression(const Type &left, const Type &right, const string &op) {
    if (left == error || right == error) {
        return error;
    }
    Type promote_left = left.promote();
    Type promote_right = right.promote();
    if ((promote_left == promote_right) && promote_left.isValue()) {
        return integer;
    }
    report(E4, op);
    return error;
}

Type checkLessThan(const Type &left, const Type &right) {
    return checkRelationalExpression(left, right, "<");
}

Type checkGreaterThan(const Type &left, const Type &right) {
    return checkRelationalExpression(left, right, ">");
}

Type checkLessThanEquals(const Type &left, const Type &right) {
    return checkRelationalExpression(left, right, "<=");
}

Type checkGreaterThanEquals(const Type &left, const Type &right) {
    return checkRelationalExpression(left, right, ">=");
}

Type checkMultiplicativeExpression(const Type &left, const Type &right, const string &op) {
    if (left == error || right == error) {
        return error;
    }
    Type promote_left = left.promote();
    Type promote_right = right.promote();
    if (promote_left.isInteger() && promote_right.isInteger()) {
        return integer;
    }
    report(E4, op);
    return error;
}

Type checkMultiply(const Type &left, const Type &right) {
    return checkMultiplicativeExpression(left, right, "*");
}

Type checkDivide(const Type &left, const Type &right) {
    return checkMultiplicativeExpression(left, right, "/");
}

Type checkMod(const Type &left, const Type &right) {
    return checkMultiplicativeExpression(left, right, "%");
}

Type checkPostfixExpression(const Type &left, const Type &right) {
    if (left == error || right == error) {
        return error;
    }
    Type promote_left = left.promote();
    Type promote_right = right.promote();
    if (promote_left.isPointer() && promote_left != Type(VOID, 1) && promote_right.isInteger()) {
        return Type(promote_left.specifier(), promote_left.indirection()-1);
    }
    report(E4, "[]");
    return error;
}

Type checkNegate(const Type &right) {
    if (right == error) {
        return error;
    }
    Type promote_right = right.promote();
    if (promote_right.isInteger()) {
        return integer;
    }
    report(E5, "-");
    return error;
}

Type checkNot(const Type &right) {
    if (right == error) {
        return error;
    }
    Type promote_right = right.promote();
    if (promote_right.isValue()) {
        return integer;
    }
    report(E5, "!");
    return error;
}

Type checkAddress(const Type &right, const bool &lvalue) {
    if (right == error) {
        return error;
    }
    if (lvalue) {
        return Type(right.specifier(), right.indirection()+1);
    }
    report(E3);
    return error;
}

Type checkDereference(const Type &right) {
    if (right == error) {
        return error;
    }
    Type promote_right = right.promote();
    if (promote_right.isPointer() && promote_right.specifier() != VOID) {
        return Type(promote_right.specifier(), promote_right.indirection()-1);
    }
    report(E5, "*");
    return error;
}

Type checkSizeof(const Type &right) {
    if (right == error) {
        return error;
    }
    if (right.isValue()) {
        return integer;
    }
    report(E5, "sizeof");
    return error;
}

Type checkAdd(const Type &left, const Type &right) {
    if (left == error || right == error) {
        return error;
    }
    Type promote_left = left.promote();
    Type promote_right = right.promote();
    if (promote_left.isInteger() && promote_right.isInteger()) {
        return integer;
    }
    else if (promote_left.isPointer() && promote_left != Type(VOID, 1) && promote_right.isInteger()) {
        return Type(promote_left.specifier(), promote_left.indirection());
    }
    else if (promote_left.isInteger() && promote_right.isPointer() && promote_right != Type(VOID, 1)) {
        return Type(promote_right.specifier(), promote_right.indirection());
    }
    report(E4, "+");
    return error;
}

Type checkSub(const Type &left, const Type &right) {
    if (left == error || right == error) {
        return error;
    }
    Type promote_left = left.promote();
    Type promote_right = right.promote();
    if (promote_left.isInteger() && promote_right.isInteger()) {
        return integer;
    }
    else if (promote_left.isPointer() && promote_left.specifier() != VOID && promote_right.isInteger()) {
        return Type(promote_left.specifier(), promote_left.indirection());
    }
    else if (promote_left.isPointer() && promote_right.isPointer() && (promote_left.specifier() != VOID) && (promote_left.specifier() == promote_right.specifier())) {
        return Type(promote_right.specifier(), promote_right.indirection());
    }
    report(E4, "-");
    return error;
}

Type checkIf(const Type &left) {
     if (left == error) {
         return error;
     }
     else if (left.isValue()) {
         return left;
     }
     report(E2);
     return error;
}

Type checkFor(const Type &left) {
    if (left == error) {
        return error;
    }
    else if (left.isValue()) {
        return left;
    }
    report(E2);
    return error;
}

Type checkWhile(const Type &left) {
    if (left == error) {
        return error;
    }
    else if (left.isValue()) {
        return left;
    }
    report(E2);
    return error;
}

Type checkReturn(const Type &left, const Type &enclosing) {
    if (left == error || enclosing == error) {
        return error;
    }
    if (left.isCompatibleWith(enclosing)) {
        return left;
    }
    report(E1);
    return error;
}

Type checkAssignment(const Type &left, const Type &right, bool &lvalue) {
    if (left == error || right == error) {
        return error;
    }
    else if (!lvalue) {
        report(E3);
        return error;
    }
    else if (left.isCompatibleWith(right)) {
        return left;
    }
    report(E4, "=");
    return error;
}

Type checkFunction(const Type &left, Parameters *params) {
    if (left == error) {
        return error;
    }
    if (!left.isFunction()) {
        report(E6);
        return error;
    }
    if (left.parameters() == nullptr) {
        return Type(left.specifier(), left.indirection());
    }
    if (left.parameters()->size() != params->size()) {
        report(E7);
        return error;
    }
    for (unsigned i = 0; i < params->size(); i++) { // at this point, parameters have same length
        Type param1 = (*left.parameters())[i].promote();
        Type param2 = (*params)[i].promote();
        if (!param1.isCompatibleWith(param2)) {
            report(E7);
            return error;
        }
    }
    return Type(left.specifier(), left.indirection());
}