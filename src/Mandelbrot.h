#ifndef MANDELBROT
#define MANDELBROT

#include "Benchmark.h"
#include "som/Error.cpp"

using namespace std;

class Mandelbrot : public Benchmark {

    private:
        static bool verifyResult(int result, int innerIterations);
        static int mandelbrot(int size);

    public:

        bool innerBenchmarkLoop(int innerIterations) override;
        any benchmark() override;
        bool verifyResult(any result) override;
};

#endif //MANDELBROT