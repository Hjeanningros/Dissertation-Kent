#include <vector>
#include "Benchmark.cpp"

class Storage : public Benchmark
{
    private:
        int _count;

        std::vector<int> buildTreeDepth(int depth) {
            _count++;

            if (depth == 1) {
                std::vector<int> node = std::vector<int>(rand() % 10 + 1);
                return node;
            } else {
                std::vector<std::vector<int>> arr = std::vector<std::vector<int>>(4);;

                for(int i = 0; i < 4; i++) {
                    arr[i] = buildTreeDepth(depth - 1);
                }
                return arr;
            }
        }

    public:
        int benchmark() override {
            srand(time(0));
            buildTreeDepth(7, random);
            return _count;
        }

        bool verifyResult(int result) override {
            return 5461 == (int)result;
        }
};



public final class Storage extends Benchmark {

  private int count;

  @Override
  public Object benchmark() {
    Random random = new Random();
    count = 0;
    buildTreeDepth(7, random);
    return count;
  }

  private Object buildTreeDepth(final int depth, final Random random) {
    count++;
    if (depth == 1) {
      return new Object[random.next() % 10 + 1];
    } else {
      Object[] arr = new Object[4];
      Arrays.setAll(arr, v -> buildTreeDepth(depth - 1, random));
      return arr;
    }
  }
