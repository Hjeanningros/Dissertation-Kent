#ifndef PERMUTE
#define PERMUTE

#include "Benchmark.h"

using namespace std;

class Permute : public Benchmark
{
    private:
        int _count;
        int *_v;

        void swap(int i, int j);

    public:
        any benchmark() override;
        void permute(int n);
        bool verifyResult(any result) override;
};

#endif //PERMUTE