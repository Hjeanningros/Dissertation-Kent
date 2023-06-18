#include <vector>
#include "Benchmark.cpp"



class Storage2 : public Benchmark
{
    private:

        class Object {
            public: 
                std::vector<std::shared_ptr<Object>> _child;
                int _val;

                Object(int v) {
                    _val = v;
                }
        };


        int _count;

        std::shared_ptr<Object> buildTreeDepth(int depth) {
            _count++;

            if (depth == 1) {
                return std::make_shared<Object>(rand() % 10 + 1);
            } else {
                std::shared_ptr<Object> arr;

                for(int i = 0; i < 4; i++) {
                    arr->_child.push_back(buildTreeDepth(depth - 1));
                }
                return arr;
            }
        }

    public:
        int benchmark() override {
            srand(time(0));
            buildTreeDepth(7);
            return _count;
        }

        bool verifyResult(int result) override {
            return 5461 == (int)result;
        }
};