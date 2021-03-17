#ifndef GENERATOR_H
#define GENERATOR_H

# include "checker.h"
# include "lexer.h"
# include "Scope.h"
# include "Tree.h"
# include "Type.h"

void generateGlobals(const Symbols &global_vars);

#endif /* GENERATOR_H */
