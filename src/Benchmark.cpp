
#include "Benchmark.h"

bool Benchmark::innerBenchmarkLoop(int innerIterations) {
    for (int i = 0; i < innerIterations; i++) {
        if (!verifyResult(benchmark())) {
            return false;
        }
    }
    return true;
}
