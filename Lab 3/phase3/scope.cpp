# include "scope.h"

using namespace std;

Scope::Scope(Scope *enclosing)
    : _enclosing(enclosing)
{
}

void Scope::insert(Symbol *symbol) {
    _symbols.push_back(symbol);
}

/* void Scope::remove(const string &name) {

} */

Symbol* Scope::find(const string &name) const { // check for symbol name within local scope
    for (unsigned i = 0; i < _symbols.size(); i++) {
        if (_symbols[i]->name() == name) {
            return _symbols[i];
        }
    }
    return nullptr;
}

Symbol* Scope::lookup(const string &name) const { // check for symbol name within global or parent scope(s)
    // basically a linked list
    Symbol *symbol;

    if ((symbol = find(name)) != nullptr) {
        return symbol;
    }

    return _enclosing != nullptr ? _enclosing->lookup(name) : nullptr;
}