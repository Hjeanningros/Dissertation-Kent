#include "UnaryConstraint.cpp"
#include "Planner.cpp"

using namespace std;

namespace deltablue {
    class StayConstraint : public UnaryConstraint {
        public:

        StayConstraint(shared_ptr<Variable> v, shared_ptr<Strength::Sym> strength,
            shared_ptr<Planner> planner) : UnaryConstraint(v, strength, planner) {}

        void execute() override {}
        
    }
}