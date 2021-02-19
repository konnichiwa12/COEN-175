#ifndef CHECKER_H
#define CHECKER_H

# include <iostream>
# include <string>
# include "type.h"

typedef std::string string;

// output functions for when scopes open and close
void openScope();
void closeScope();

// when encountering a declaration/definition
void declareVariable(const string &name, const Type &type);
void declareArray(const string &name, const Type &type);
void declareFunction(const string &name, const Type &type);
void defineFunction(const string &name, const Type &type);

// call when encountering an identifier
void checkID(const string &id, const Type &type);

#endif /* CHECKER_H */
