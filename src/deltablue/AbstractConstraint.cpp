#include "Planner.cpp"

using namespace std;

namespace deltablue {

    AbstractConstraint::AbstractConstraint(shared_ptr<Strength::Sym> strength) {
        _strength = Strength::of(strength);
    }

    shared_ptr<Strength> AbstractConstraint::getStrength() {
        return _strength;
    }

    bool AbstractConstraint::isInput() {
        return false;
    }

    void AbstractConstraint::addConstraint(shared_ptr<Planner> planner) {
        addToGraph();
        planner->incrementalAdd(make_shared<AbstractConstraint>(_strength));
    }


    void AbstractConstraint::destroyConstraint(shared_ptr<Planner> planner) {
        if (isSatisfied()) {
            planner->incrementalRemove(this);
        }
        removeFromGraph();
    }

    bool AbstractConstraint::inputsKnown(int mark) {
        return !inputsHasOne([&mark](shared_ptr<Variable> v) -> bool {
            return !(v->getMark() == mark || v->getStay() || v->getDeterminedBy() == nullptr);
        });
    }

    shared_ptr<AbstractConstraint> AbstractConstraint::satisfy(int mark, shared_ptr<Planner> planner) {
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
            if (_strength->sameAs(Strength::required())) {
                throw Error("Could not satisfy a required constraint");
            }   
        }
        return overridden;
    }
}