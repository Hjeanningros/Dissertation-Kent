#include "Strength.h"

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

    struct SharedPtrSymComparator {
        bool operator()(shared_ptr<Strength::Sym> lhs, shared_ptr<Strength::Sym> rhs) {
            return lhs->customHash() == rhs->customHash();
        }
    };

    extern map<shared_ptr<Strength::Sym>, int> Strength::_strengthTable = {
        {make_shared<Sym>(0), -10000},
        {make_shared<Sym>(1), -800},
        {make_shared<Sym>(2), -600},
        {make_shared<Sym>(3), -400},
        {make_shared<Sym>(4), -200},
        {make_shared<Sym>(5), 0},
        {make_shared<Sym>(6), 500},
        {make_shared<Sym>(7), 10000}
    };


    extern map<shared_ptr<Strength::Sym>, shared_ptr<Strength>> Strength::_strengthConstant = {
        {make_shared<Sym>(0), make_shared<Strength>(make_shared<Sym>(0))},
        {make_shared<Sym>(1), make_shared<Strength>(make_shared<Sym>(1))},
        {make_shared<Sym>(2), make_shared<Strength>(make_shared<Sym>(2))},
        {make_shared<Sym>(3), make_shared<Strength>(make_shared<Sym>(3))},
        {make_shared<Sym>(4), make_shared<Strength>(make_shared<Sym>(4))},
        {make_shared<Sym>(5), make_shared<Strength>(make_shared<Sym>(5))},
        {make_shared<Sym>(6), make_shared<Strength>(make_shared<Sym>(6))},
        {make_shared<Sym>(7), make_shared<Strength>(make_shared<Sym>(7))}
    };
    
    // Set static member variable
    extern shared_ptr<Strength::Sym> Strength::ABSOLUTE_STRONGEST = make_shared<Sym>(0);
    extern shared_ptr<Strength::Sym> Strength::REQUIRED           = make_shared<Sym>(1);
    extern shared_ptr<Strength::Sym> Strength::STRONG_PREFERRED   = make_shared<Sym>(2);
    extern shared_ptr<Strength::Sym> Strength::PREFERRED          = make_shared<Sym>(3);
    extern shared_ptr<Strength::Sym> Strength::STRONG_DEFAULT     = make_shared<Sym>(4);
    extern shared_ptr<Strength::Sym> Strength::DEFAULT            = make_shared<Sym>(5);
    extern shared_ptr<Strength::Sym> Strength::WEAK_DEFAULT       = make_shared<Sym>(6);
    extern shared_ptr<Strength::Sym> Strength::ABSOLUTE_WEAKEST   = make_shared<Sym>(7);

    extern shared_ptr<Strength> Strength::_absoluteWeakest = of(make_shared<Sym>(7));
    extern shared_ptr<Strength> Strength::_required = of(make_shared<Sym>(1));

}
