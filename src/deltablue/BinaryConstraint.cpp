#include "AbstractConstraint.h"

using namespace std;

namespace deltablue {
    class BinaryConstraint: public AbstractConstraint {
        protected:
            shared_ptr<Variable> _v1;
            shared_ptr<Variable> _v2;
            Direction _direction;

        public:
            BinaryConstraint(shared_ptr<Variable> var1, shared_ptr<Variable> var2,
                shared_ptr<Strength::Sym> strength/*, shared_ptr<Planner> planner*/) : AbstractConstraint(strength){ /// don't use planner ??? Probleme with "this"
                _v1 = var1;
                _v2 = var2;
                _direction = NULLPTR;
            }

            bool isSatisfied() override {
                return _direction != NULLPTR;
            }

            void addToGraph() override {
                _v1->addConstraint(std::make_shared<BinaryConstraint>(_v1, _v2, _strength));
                _v2->addConstraint(std::make_shared<BinaryConstraint>(_v1, _v2, _strength));
                _direction = NULLPTR;
            }

            void removeFromGraph() override {
                _v1->removeConstraint(std::make_shared<BinaryConstraint>(_v1, _v2, _strength));
                _v2->removeConstraint(std::make_shared<BinaryConstraint>(_v1, _v2, _strength));
                _direction = NULLPTR;
            }

            Direction chooseMethod(int mark) override {
                if (_v1->getMark() == mark) {
                    if (_v2->getMark() != mark && _strength->stronger(_v2->getWalkStrength())) {
                        _direction = FORWARD;
                        return _direction;
                    } else {
                        _direction = NULLPTR;
                        return _direction;
                    }
                }

                if (_v2->getMark() == mark) {
                    if (_v1->getMark() != mark && _strength->stronger(_v1->getWalkStrength())) {
                        _direction = BACKWARD;
                        return _direction;
                    } else {
                        _direction = NULLPTR;
                        return _direction;
                    }
                }

                if (_v1->getWalkStrength()->weaker(_v2->getWalkStrength())) {
                    if (_strength->stronger(_v1->getWalkStrength())) {
                        _direction = BACKWARD;
                        return _direction;
                    } else {
                        _direction = NULLPTR;
                        return _direction;
                    }
                } else {
                    if (_strength->stronger(_v2->getWalkStrength())) {
                        _direction = FORWARD;
                        return _direction;
                    } else {
                        _direction = NULLPTR;
                        return _direction;
                    }
                }
            }

            void inputsDo(function<void(shared_ptr<Variable>)> fn) override {
                if (_direction == FORWARD) {
                    fn(_v1);
                } else {
                    fn(_v2);
                }
            }

            bool inputsHasOne(function<bool(shared_ptr<Variable>)> fn) override {
                if (_direction == FORWARD) {
                    return fn(_v1);
                } else {
                    return fn(_v2);
                }
            }

            void markUnsatisfied() override {
                _direction = NULLPTR;
            }

            shared_ptr<Variable> getOutput() override {
                return _direction == FORWARD ? _v2 : _v1;
            }

            void recalculate() override {
                shared_ptr<Variable> in;
                shared_ptr<Variable> out;

                if (_direction == FORWARD) {
                    in = _v1; 
                    out = _v2;
                } else {
                    in = _v2; 
                    out = _v1;
                }

                out->setWalkStrength(_strength->weakest(in->getWalkStrength()));
                out->setStay(in->getStay());
                if (out->getStay()) {
                    execute();
                }
            }
    };
}