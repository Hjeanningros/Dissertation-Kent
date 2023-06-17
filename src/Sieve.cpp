#include <vector>
#include "Benchmark.cpp"

class Sieve : public Benchmark
{
    public:
   
    int benchmark() override {
        std::vector<bool> flags(5000, true);
        return sieve(flags, 5000);
    }

    int sieve(std::vector<bool>& flags, int size) {
        int primeCount = 0;

        for (int i = 2; i <= size; i++) {
            if (flags[i - 1]) {
                primeCount++;
                int k = i + i;
                while (k <= size) {
                    flags[k - 1] = false;
                    k += i;
                }
            }
        }
        return primeCount;
    }
    
    bool verifyResult(int result) override {
        return 669 == result;
    }
};