#include "BinaryConstraint.h"
#include "Variable.h"
#include <memory>

using namespace std;

namespace deltablue {
    class ScaleConstraint :public BinaryConstraint {
        private:
            shared_ptr<Variable> _scale;
            shared_ptr<Variable> _offset;

        public:
            ScaleConstraint(shared_ptr<Variable> src, shared_ptr<Variable> scale, shared_ptr<Variable> offset,
                            shared_ptr<Variable> dest, shared_ptr<Strength::Sym> strength, shared_ptr<Planner> planner)
                : BinaryConstraint(src, dest, strength/*, planner*/) {
                _strength = Strength::of(strength);
                _v1 = src;
                _v2 = dest;
                _direction = NONE;
                _scale = scale;
                _offset = offset;

                addConstraint(planner);
            }

            void addToGraph() override {

                //_v1->addConstraint(make_shared<ScaleConstraint>(_v1, _scale, _offset, _v2, _strenght));
                _v1->addConstraint(shared_ptr<ScaleConstraint>(this));
                _v2->addConstraint(shared_ptr<ScaleConstraint>(this));
                _scale->addConstraint(shared_ptr<ScaleConstraint>(this));
                _offset->addConstraint(shared_ptr<ScaleConstraint>(this));
                _direction = NONE;
            }

            void removeFromGraph() override {
                if (_v1 != nullptr) 
                    _v1->removeConstraint(shared_ptr<ScaleConstraint>(this));
                if (_v2 != nullptr) 
                    _v2->removeConstraint(shared_ptr<ScaleConstraint>(this));
                if (_scale != nullptr) 
                    _scale->removeConstraint(shared_ptr<ScaleConstraint>(this));
                if (_offset != nullptr) 
                    _offset->removeConstraint(shared_ptr<ScaleConstraint>(this));
                _direction = NONE;
            }

            void execute() override {
                if (_direction == FORWARD) {
                _v2->setValue(_v1->getValue() * _scale->getValue() + _offset->getValue());
                } else {
                _v1->setValue((_v2->getValue() - _offset->getValue()) / _scale->getValue());
                }
            }

            void inputsDo(function<void(shared_ptr<Variable>)> fn) override {
                if (_direction == FORWARD) {
                    fn(_v1);
                    fn(_scale);
                    fn(_offset);
                } else {
                    fn(_v2);
                    fn(_scale);
                    fn(_offset);
                }
            }

            void recalculate() override {
                shared_ptr<Variable> in;
                shared_ptr<Variable> out;

                if (_direction == FORWARD) {
                    in = _v1;
                    out = _v2;
                } else {
                    out = _v1;
                    in = _v2;
                }

                out->setWalkStrength(_strength->weakest(in->getWalkStrength()));
                out->setStay(in->getStay() && _scale->getStay() && _offset->getStay());
                if (out->getStay()) {
                    execute(); // stay optimization
                }
            }
    };
}