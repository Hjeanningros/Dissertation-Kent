#ifndef BOUNCE
#define BOUNCE

#include "Benchmark.h"
#include "som/Random.cpp"

using namespace std;

class Bounce : public Benchmark 
{
    private:
        class Ball {
            private:
                int _x;
                int _y;
                int _xVel;
                int _yVel;

            public:
                Ball() = default;
                Ball(shared_ptr<Random> random);

                bool bounce();
        };


    public:
        any benchmark() override;
        bool verifyResult(any result) override;
};

#endif //BOUNCE