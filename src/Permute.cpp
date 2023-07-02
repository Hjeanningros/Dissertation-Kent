#include <vector>
#include "Benchmark.cpp"

class Permute : public Benchmark
{
    private:
        int _count;
        std::vector<int> _v{std::vector<int>(5)};


        void swap(int i, int j) {
            int tmp = _v[i];
            _v[i] = _v[j];
            _v[j] = tmp;
        }

    public:
        std::any benchmark() override {
            _count = 0;
            permute(6);
            return _count;
        }

        void permute(int n) {
            _count++;
            if (n != 0) {
                int n1 = n - 1;
                permute(n1);
                for (int i = n1; i >= 0; i--) {
                    swap(n1, i);
                    permute(n1);
                    swap(n1, i);
                }
            }
        }
    
        bool verifyResult(std::any result) override {
            int result_cast = std::any_cast<int>(result);
            return result_cast == 8660;
        }
};