#include "Benchmark.h"
#include "som/Error.cpp"
#include "deltablue/Planner.h"
#include <iostream>

using namespace std;

namespace deltablue {
    class DeltaBlue : public Benchmark {
        public:
            bool innerBenchmarkLoop(int innerIterations) override;
            any benchmark() override;
            bool verifyResult(any result) override;
    };
}   