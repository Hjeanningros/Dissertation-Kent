#ifndef ABSTRACTCONSTRAINT
#define ABSTRACTCONSTRAINT

#include <functional>
#include <memory>
#include "Strength.h"
#include "../som/Error.cpp"
#include "Variable.h"
#include "Direction.h"

using namespace std;

namespace deltablue {

    class Planner;

    class AbstractConstraint {
        protected:
            shared_ptr<Strength> _strength;

        public:


            AbstractConstraint(shared_ptr<Strength::Sym> strength);

            shared_ptr<Strength> getStrength();
            bool isInput();
            void addConstraint(shared_ptr<Planner> planner);
            void destroyConstraint(shared_ptr<Planner> planner);
            bool inputsKnown(int mark);
            shared_ptr<AbstractConstraint> satisfy(int mark, shared_ptr<Planner> planner);

            virtual bool isSatisfied() = 0;
            virtual void addToGraph() = 0;
            virtual void removeFromGraph() = 0;
            virtual Direction chooseMethod(int mark) = 0;
            virtual void execute() = 0;
            virtual void inputsDo(function<void(shared_ptr<Variable>)> fn) = 0;
            virtual bool inputsHasOne(function<bool(shared_ptr<Variable> )> fn) = 0;
            virtual void markUnsatisfied() = 0;
            virtual shared_ptr<Variable> getOutput() = 0;
            virtual void recalculate() = 0;


    };
}

#endif //ABSTRACTCONSTRAINT