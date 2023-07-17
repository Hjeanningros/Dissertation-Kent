#include "Plan.h"

using namespace std;

namespace deltablue {

    Plan::Plan() : vector<shared_ptr<AbstractConstraint>>() {}

    void Plan::execute() {
        for (const auto& constraint : *this) {
            constraint->execute();
        }
    }
}