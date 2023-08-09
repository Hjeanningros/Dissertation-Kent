#ifndef SIEVE
#define SIEVE

#include "Benchmark.h"

using namespace std;

class Sieve : public Benchmark
{
    public:
   
    any benchmark() override;
    static int sieve(bool *flags, int size);
    bool verifyResult(any result) override;
};

#endif //SIEVE