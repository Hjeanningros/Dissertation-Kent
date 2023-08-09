#ifndef STORAGE
#define STORAGE

#include "Benchmark.h"
#include "som/Random.cpp"

using namespace std;

class Storage : public Benchmark
{
    private:
        int _count;
        void* buildTreeDepth(int depth, shared_ptr<Random> random);

    public:
        any benchmark() override;
        bool verifyResult(any result) override;
};

#endif //STORAGE
