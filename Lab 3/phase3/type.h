#ifndef TYPE_H
#define TYPE_H

#include <ostream>
#include <vector>

typedef std::vector<class Type> Parameters;

class Type {
    int _specifier;
    unsigned _indirection;
    unsigned _length; // only used if it's an array
    Parameters *_parameters; // only used if it's a function; nullptr = (), [] = (void)

    enum Kind { ARRAY, ERROR, FUNCTION, SCALAR } _kind; // ERROR is for future functions

public:
    Type(); // error
    Type(int specifier, unsigned indirection = 0); // scalar
    Type(int specifier, unsigned indirection, unsigned length); // array
    Type(int specifier, unsigned indirection, Parameters *parameters); // function

    bool isArray() const { return _kind == ARRAY; };
    bool isError() const { return _kind == ERROR; };
    bool isFunction() const { return _kind == FUNCTION; };
    bool isScalar() const { return _kind == SCALAR; };

    int specifier() const { return _specifier; }; // const saves a lot of time for the compiler
    unsigned indirection() const { return _indirection; };
    unsigned length() const { return _length; };
    Parameters *parameters() const { return _parameters; };

    Kind kind() const { return _kind; };

    bool operator ==(const Type &that) const; // pass by reference to save memory, const helps compiler and programmer understand what's going on
    bool operator !=(const Type &that) const;
};

std::ostream &operator <<(std::ostream &ostr, const Type &type); // non-member function

#endif /* TYPE_H */