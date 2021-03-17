/*
 * File:	generator.h
 *
 * Description:	This file contains the function declarations for the code
 *		generator for Simple C.  Most of the function declarations
 *		are actually member functions provided as part of Tree.h.
 */

# ifndef GENERATOR_H
# define GENERATOR_H
# include "checker.h"
# include "label.h"
# include "lexer.h"
# include "machine.h"
# include "Register.h"
# include "Scope.h"
# include "Tree.h"
# include "Type.h"

typedef std::string string;

void generateGlobals(Scope *scope);

void load(Expression *expr, Register *reg);
void assign(Expression *expr, Register *reg);
Register *getreg();

void compute(Expression *result, Expression *left, Expression *right, const string &opcode);
void compare(Expression *result, Expression *left, Expression *right, const string &opcode);

# endif /* GENERATOR_H */
