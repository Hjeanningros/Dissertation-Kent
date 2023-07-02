#include <vector>
#include "Benchmark.cpp"
#include "som/Random.cpp"
class Storage : public Benchmark
{
    private:
        int _count;

        std::vector<void> buildTreeDepth(int depth, Random random) {
            _count++;

            if (depth == 1) {
                std::vector<void> node = std::vector<void>(random.next() % 10 + 1);
                return node;
            } else {
                std::vector<void> arr = std::vector<void>(4);;

                for(int i = 0; i < 4; i++) {
                    arr[i] = buildTreeDepth(depth - 1, random);
                }
                return arr;
            }
        }

    public:
        std::any benchmark() override {
            Random random = Random();
            _count = 0;
            buildTreeDepth(7, random);
            return _count;
        }

        bool verifyResult(std::any result) override {
            bool result_cast = std::any_cast<int>(result);
            return 5461 == result_cast;
        }
};

