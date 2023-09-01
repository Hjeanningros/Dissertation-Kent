#include "DeltaBlue.h"

using namespace std;

namespace deltablue {
    bool DeltaBlue::innerBenchmarkLoop(int innerIterations) {
        Planner::chainTest(innerIterations);
        Planner::projectionTest(innerIterations);
        return true;
    } 

    any DeltaBlue::benchmark() {
        throw Error("should never be reached");
    }
    
    bool DeltaBlue::verifyResult(any result) {
        (void)result;
        throw Error("should never be reached");
    }
}   