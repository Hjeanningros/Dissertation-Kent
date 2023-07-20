#ifndef PLANNER
#define PLANNER

#include <vector>
#include <memory>
#include <algorithm>
#include "AbstractConstraint.h"
#include "EqualityConstraint.cpp"
#include "ScaleConstraint.cpp"
#include "Plan.h"

#include "StayConstraint.h"
#include "EditConstraint.h"

using namespace std;

namespace deltablue {

    class Planner {
        private: 
            int _currentMark;

        public:
            Planner();

            void incrementalAdd(shared_ptr<AbstractConstraint> c);
            void incrementalRemove(shared_ptr<AbstractConstraint> c);
            shared_ptr<Plan> extractPlanFromConstraints(vector<shared_ptr<AbstractConstraint>> constraints);
            shared_ptr<Plan> makePlan(vector<shared_ptr<AbstractConstraint>> sources);
            void propagateFrom(shared_ptr<Variable> v);
            void addConstraintsConsumingTo(shared_ptr<Variable> v, vector<shared_ptr<AbstractConstraint>> coll);
            bool addPropagate(shared_ptr<AbstractConstraint> c, int mark);

            void change(shared_ptr<Variable> var, int newValue);

            void constraintsConsuming(shared_ptr<Variable> v, function<void(shared_ptr<AbstractConstraint>)> fn);

            int newMark();

            vector<shared_ptr<AbstractConstraint>> removePropagateFrom(shared_ptr<Variable> out);

            static void chainTest(int n);
            void projectionTest(int n);
    };
}

#endif //PLANNER