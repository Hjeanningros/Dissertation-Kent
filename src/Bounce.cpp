#include <cstdlib>
#include <vector>
#include <memory>
#include "Benchmark.cpp"
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
            
            Ball(shared_ptr<Random> random) {
                _x = random->next() % 500;
                _y = random->next() % 500;
                _xVel = (random->next() % 300) - 150;
                _yVel = (random->next() % 300) - 150;
            }

            bool bounce() {
                int xLimit = 500;
                int yLimit = 500;
                bool bounced = false;

                _x += _xVel;
                _y += _yVel;

                if (_x > xLimit) {
                    _x = xLimit;
                    _xVel = 0 - abs(_xVel);
                    bounced = true;
                } 
                if (_x < 0) {
                    _x = 0;
                    _xVel = abs(_xVel);
                    bounced = true;
                }
                if (_y > yLimit) {
                    _y = yLimit;
                    _yVel = 0 - abs(_yVel);
                    bounced = true;
                } 
                if (_y < 0) {
                    _y = 0;
                    _yVel = abs(_yVel);
                    bounced = true;
                }
                return bounced;
            }
        };


    public:
        any benchmark() override {
            shared_ptr<Random> random = make_shared<Random>();
            int ballCount = 100;
            int bounces = 0;
            vector<Ball> balls;

            for (int i = 0; i < 100; i++)
                balls.push_back(Ball(random));

            for (int i = 0; i < 50; i++) {
                for (Ball& ball: balls) {
                    if (ball.bounce()) {
                        bounces += 1;
                    }
                }
            }

            return bounces;
        }
        
        bool verifyResult(any result) override {
            int result_cast = any_cast<int>(result);
            return 1331 == result_cast;
        }
};