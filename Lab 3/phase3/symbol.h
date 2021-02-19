#ifndef SYMBOL_H
#define SYMBOL_H

# include <string>
# include "type.h"

class Symbol {
    typedef std::string string;
    string _name;
    Type _type;

public:
    bool _defined;
    Symbol(string name, const Type& type, bool defined = false);
    const string &name() const { return _name; }; // must pass by const reference since it's private
    const Type &type() const { return _type; };
};

#endif /* SYMBOL_H */
