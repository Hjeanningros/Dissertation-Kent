#ifndef PLAN
#define PLAN

#include "AbstractConstraint.h"
#include <vector>

using namespace std;

namespace deltablue {

    class Plan : public vector<shared_ptr<AbstractConstraint>> {
        public:
            Plan();

            void execute();
    };
}

#endif //PLAN