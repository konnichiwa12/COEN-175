/*
 * File:	checker.h
 *
 * Description:	This file contains the public function declarations for the
 *		semantic checker for Simple C.
 */

# ifndef CHECKER_H
# define CHECKER_H
# include "Scope.h"

typedef std::string string;

Scope *openScope();
Scope *closeScope();

Symbol *defineFunction(const std::string &name, const Type &type);
Symbol *declareFunction(const std::string &name, const Type &type);
Symbol *declareVariable(const std::string &name, const Type &type);
Symbol *checkIdentifier(const std::string &name);

Type checkLogical(const Type &left, const Type &right, const string &op);
Type checkLogicalOr(const Type &left, const Type &right);
Type checkLogicalAnd(const Type &left, const Type &right);

Type checkEquality(const Type &left, const Type &right);
Type checkNotEquality(const Type &left, const Type &right);

Type checkRelationalExpression(const Type &left, const Type &right, const string &op);
Type checkLessThan(const Type &left, const Type &right);
Type checkGreaterThan(const Type &left, const Type &right);
Type checkLessThanEquals(const Type &left, const Type &right);
Type checkGreaterThanEquals(const Type &left, const Type &right);

Type checkMultiplicativeExpression(const Type &left, const Type &right, const string &op);
Type checkMultiply(const Type &left, const Type &right);
Type checkDivide(const Type &left, const Type &right);
Type checkMod(const Type &left, const Type &right);

Type checkPostfixExpression(const Type &left, const Type &right);

Type checkNegate(const Type &right);
Type checkNot(const Type &right);
Type checkAddress(const Type &right, const bool &lvalue);
Type checkDereference(const Type &right);
Type checkSizeof(const Type &right);

Type checkAdd(const Type &left, const Type &right);
Type checkSub(const Type &left, const Type &right);

Type checkIf(const Type &left);
Type checkFor(const Type &left);
Type checkWhile(const Type &left);
Type checkReturn(const Type &left, const Type &enclosing);
Type checkAssignment(const Type &left, const Type &right, bool &lvalue);
Type checkFunction(const Type &left, Parameters *params);


# endif /* CHECKER_H */
