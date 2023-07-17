#include <memory>
#include "AbstractConstraint.h"

using namespace std;

namespace deltablue {
    class UnaryConstraint : public AbstractConstraint {
        protected:
            shared_ptr<Variable> _output; // possible output variable
            bool _satisfied;   // true if I am currently satisfied

        public:
            UnaryConstraint(shared_ptr<Variable> v, shared_ptr<Sym> strength, shared_ptr<Planner> planner)
                : AbstractConstraint(strength) {
                _output = v;
                _satisfied = false;
                addConstraint(planner);
            }

            virtual ~UnaryConstraint() {}

            bool isSatisfied() const override {
                return _satisfied;
            }

            // Add myself to the constraint graph.
            void addToGraph() override {
                output->addConstraint(this);
                _satisfied = false;
            }

            // Remove myself from the constraint graph.
            void removeFromGraph() override {
                if (_output != nullptr) {
                    _output->removeConstraint(this);
                }
                _satisfied = false;
            }

            shared_ptr<Direction> chooseMethod(int mark) override {
                _satisfied = (_output->getMark() != mark) &&
                            _strength.stronger(_output->getWalkStrength());
                return nullptr;
            }

            virtual void execute() = 0;

            void inputsDo(function<void(shared_ptr<Variable>)> fn) override {
                // I have no input variables
            }

            bool inputsHasOne(function<void(shared_ptr<Variable>)> fn) override {
                return false;
            }

            void markUnsatisfied() override {
                _satisfied = false;
            }

            shared_ptr<Variable> getOutput() override {
                return _output;
            }

            void recalculate() override {
                _output->setWalkStrength(_strength);
                _output->setStay(!isInput());
                if (_output->getStay()) {
                    execute();
                }
            }

    };
}