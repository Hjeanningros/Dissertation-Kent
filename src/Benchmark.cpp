#ifndef B_OOP_400_STG_4_1_ARCADE_NICOLAS_SCHNEIDER_CLOCK_HPP
#define B_OOP_400_STG_4_1_ARCADE_NICOLAS_SCHNEIDER_CLOCK_HPP

#include <memory>

class Benchmark {
    public: 
        virtual int benchmark() = 0; // std::any instead ? return std::shared_ptr<void> ? Try without shared pointer
        virtual bool verifyResult(int result) = 0; // as argument std::shared_ptr<void> ?

        virtual bool innerBenchmarkLoop(int innerIterations) {
            for (int i = 0; i < innerIterations; i++) {
                if (!verifyResult(benchmark())) {
                    return false;
                }
            }
            return true;
        }

        ~Benchmark() {
        }
};

#endif //B_OOP_400_STG_4_1_ARCADE_NICOLAS_SCHNEIDER_CLOCK_HPP