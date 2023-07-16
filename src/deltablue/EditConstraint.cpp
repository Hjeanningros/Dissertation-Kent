#include <memory>
#include "UnaryConstraint.cpp"
#include "Planner.cpp"

using namespace std;

namespace deltablue {
    class EditConstraint: public UnaryConstraint {
        public:

            EditConstraint(shared_ptr<Variable> v, shared_ptr<Strenght::Sym> strength, shared_ptr<Planner> planner)
                        : UnaryConstraint(v, strength, planner) {
            }

            bool isInput() override {
                return true;
            }

            void execute() override {
                // Edit constraints do nothing.
            }
    };
}