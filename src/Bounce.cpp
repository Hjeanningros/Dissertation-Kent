#include "Benchmark.cpp"
#include <string>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>

class Bounce : public Benchmark 
{
    private:

        class Ball {
            private:
                int _x;
                int _y;
                int _xVel;
                int _yVel;
        
            Ball() {

                _x = rand() % 500;
                _y = rand() % 500;
                _xVel = (rand() % 300) - 150;
                _yVel = (rand() % 300) - 150;
            }

            public:
            
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
                    _x = 0;
                    _yVel = abs(_yVel);
                    bounced = true;
                }
                return bounced;
            }

        };


    public:
        int benchmark() override {
            srand(time(0));

            int ballCount = 100;
            int bounces = 0;
            std::vector<std::shared_ptr<Ball>> balls;

            for (int i = 0; i < ballCount; i++)
                balls.push_back(std::make_shared<Ball>());

            for (int i = 0; i < 50; i++) {
                for (std::shared_ptr<Ball>& ball: balls) {
                    if (ball->bounce()) {
                        bounces += 1;
                    }
                }
            }

            return bounces;
        }
        
        bool verifyResult(int result) override {
            return 1331 == result;
        }

        ~Bounce() {
        }
};