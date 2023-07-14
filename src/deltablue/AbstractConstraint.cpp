#include "Strength.cpp"
#include <memory>
#include "../som/Error.cpp"


namespace deltablue {
    class AbstractConstraint {
        protected:
            shared_ptr<Strength> _strength;

        public:


            AbstractConstraint(shared_ptr<Sym> strength) {
                _strength = Strength->of(strength);
            }

            shared_ptr<Strength> getStrength() {
                return _strength;
            }

            boolean isInput() {
                return false;
            }

            virtual bool isSatisfied() = 0;

            void addConstraint(shared_ptr<Planner> planner) {
                addToGraph();
                planner.incrementalAdd(make_shared<AbstractConstraint>(_strength));
            }

            virtual void addToGraph() = 0;

            void destroyConstraint(shared_ptr<Planner> planner) {
                if (isSatisfied()) {
                    planner->incrementalRemove(this);
                }
                removeFromGraph();
            }

            virtual void removeFromGraph() = 0;

            virtual shared_ptr<Direction> chooseMethod(int mark) = 0;

            virtual void execute() = 0;

            virtual void inputsDo(function<void(shared_ptr<Variable>)> fn) = 0;
            virtual bool inputsHasOne(function<void(shared_ptr<Variable> )> fn) = 0;

            bool inputsKnown(int mark) {
                return !inputsHasOne([&](shared_ptr<Variable> v) -> void {
                    return !(v->getMark() == mark || v->getStay() || v->getDeterminedBy() == nullptr);
                });
            }

            virtual void markUnsatisfied() = 0;

            virtual Variable& getOutput() = 0;

            virtual void recalculate() = 0;

            shared_ptr<AbstractConstraint> satisfy(int mark, shared_ptr<Planner> planner) {
                shared_ptr<AbstractConstraint> overridden;

                chooseMethod(mark);

                if (isSatisfied()) {
                
                    inputsDo([&mark](shared_ptr<Variable> in) -> void { 
                        in->setMark(mark); 
                    });

                    shared_ptr<Variable> out = getOutput();
                    overridden = out->getDeterminedBy();
                    if (overridden != nullptr) {
                        overridden->markUnsatisfied();
                    }
                    out->setDeterminedBy(this);
                    if (!planner->addPropagate(this, mark)) {
                        throw Error("Cycle encountered");
                    }
                    out->setMark(mark);
                } else {
                    overridden = nullptr;
                    if (strength.sameAs(Strength::required())) {
                        throw Error("Could not satisfy a required constraint");
                    }   
                }
                return overridden;
            }
    };
}
