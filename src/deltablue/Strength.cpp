#include <unordered_map>
#include <memory>
#include <Strength.h>

using namespace std;

namespace deltablue {

    Strength::Sym::Sym(int hash) {
        _hash = hash;
    }

    int Strength::Sym::customHash() const {
          return _hash;
    }
        
    Strength::Strength(shared_ptr<Sym> symbolicValue) {
        _symbolicValue = _symbolicValue;
        _arithmeticValue = _strengthTable[symbolicValue];
    }

        
    bool Strength::sameAs(shared_ptr<Strength> s) const {
        return _arithmeticValue == s->_arithmeticValue;
    }

    bool Strength::stronger(shared_ptr<Strength> s) const {
        return _arithmeticValue < s->_arithmeticValue;
    }

    bool Strength::weaker(shared_ptr<Strength> s) const {
        return _arithmeticValue > s->_arithmeticValue;
    }

    shared_ptr<Strength> Strength::strongest(shared_ptr<Strength> s) const {
        return s->stronger(make_shared<Strength>(_symbolicValue)) ? s : make_shared<Strength>(_symbolicValue);
    }

    shared_ptr<Strength> Strength::weakest(shared_ptr<Strength> s) const {
        return s->weaker(make_shared<Strength>(_symbolicValue)) ? s : make_shared<Strength>(_symbolicValue);
    }

    int Strength::get_arithmeticValue() const {
        return _arithmeticValue;
    }

    shared_ptr<Strength> Strength::of(shared_ptr<Sym> strength) {
        return _strengthConstant[strength];
    }

    shared_ptr<Strength> Strength::absoluteWeakest() {
        return _absoluteWeakest;
    }

    shared_ptr<Strength> Strength::required() {
        return _required;
    }
}
