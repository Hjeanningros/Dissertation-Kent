
#include <memory>

class Benchmark {
    public: 
        virtual std::shared_ptr<void> _benchmark() = 0; // std::any instead ?
        virtual bool _verifyResult(std::shared_ptr<void> result) = 0;

        bool innerBenchmarkLoop(int innerIterations) {
            for (int i = 0; i < innerIterations; i++) {
                if (!_verifyResult(_benchmark())) {
                    return false;
                }
            }
            return true;
        }

        ~Benchmark() {
        }
};