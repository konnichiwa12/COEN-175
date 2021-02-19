/*
 * Ethan Paek
 * TA: Chris Desiniotis
 * COEN 175L: Mon 5:15 PM - 8:00 PM
 * 7 February 2021
 */

#include "tokens.h"
#include "type.h"

using namespace std;

Type::Type()
    : _kind(ERROR)
{
}

Type::Type(int specifier, unsigned indirection)
    : _specifier(specifier), _indirection(indirection), _kind(SCALAR)
{
}

Type::Type(int specifier, unsigned indirection, unsigned length)
        : _specifier(specifier), _indirection(indirection), _length(length), _kind(ARRAY)
{
}

Type::Type(int specifier, unsigned indirection, Parameters *parameters)
        : _specifier(specifier), _indirection(indirection), _parameters(parameters), _kind(FUNCTION)
{
}

bool Type::operator!=(const Type &that) const {
    return !operator==(that);
}

bool Type::operator==(const Type &that) const {
    if (_kind != that._kind) { // comparing apples to oranges?
        return false;
    }
    if (_kind == ERROR) { // nothing left to check if it's an error
        return true;
    }
    if (_specifier != that._specifier) {
        return false;
    }
    if (_indirection != that._indirection) {
        return false;
    }
    if (_kind == SCALAR) {
        return true;
    }
    if (_kind == ARRAY) {
        return _length == that._length;
    }
    if (!_parameters || !that._parameters) { // if parameters are undefined
        return true;
    }
    return *_parameters == *that._parameters; // check if all parameters in vectors are the same
}

std::ostream &operator <<(std::ostream &ostr, const Type &type) {
    ostr << "typespec: " << type.specifier() << " indirection: " << type.indirection() << " type: " << type.kind() << endl;
    return ostr;
}