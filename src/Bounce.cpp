#include "Benchmark.cpp"
#include <string>

class Bounce : public Benchmark 
{
    public:
        std::string test = "hahah";

        ~Bounce() {
        }
};