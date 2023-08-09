#ifndef BENCHMARK
#define BENCHMARK

#include <any>
#include <memory>
#include <iostream>
#include <algorithm>
#include <utility>

using namespace std;


class Benchmark {
    public: 
        virtual any benchmark() = 0;
        virtual bool verifyResult(any result) = 0;

        virtual bool innerBenchmarkLoop(int innerIterations);
};


#endif //BENCHMARK