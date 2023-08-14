#include "Benchmark.h"
#include "cd/Simulator.h"
#include "cd/CollisionDetector.h"
#include "som/Error.cpp"
#include <iostream>

namespace CD {
    class CD : public Benchmark {
        private:
            static int benchmark(int numAircrafts);

        public: 

            bool innerBenchmarkLoop(int innerIterations) override;

            static bool verifyResult(int actualCollisions, int numAircrafts);
            std::any benchmark() override;
            bool verifyResult(std::any result) override;
    };
};