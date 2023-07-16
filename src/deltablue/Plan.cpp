#include <vector>
#include "AbstractConstraint.h"

using namespace std;

namespace deltablue {

    class Plan : public vector<shared_ptr<AbstractConstraint>> {
        public:
            Plan() : vector<shared_ptr<AbstractConstraint>>() {}

            void execute() {
                for (const auto& constraint : *this) {
                    constraint->execute();
                }
            }
    };
}