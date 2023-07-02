#include <vector>
#include "Benchmark.cpp"

class Sieve : public Benchmark
{
    public:
   
    std::any benchmark() override {
        std::vector<bool> flags(5000, true);
        return sieve(flags, 5000);
    }

    static int sieve(std::vector<bool>& flags, int size) {
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
    
    bool verifyResult(std::any result) override {
        int result_cast = std::any_cast<int>(result);
        return 669 == result_cast;
    }
};