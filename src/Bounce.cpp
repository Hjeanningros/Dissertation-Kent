#include "Bounce.h"


Bounce::Ball::Ball(shared_ptr<Random> random) {
    _x = random->next() % 500;
    _y = random->next() % 500;
    _xVel = (random->next() % 300) - 150;
    _yVel = (random->next() % 300) - 150;
}

bool Bounce::Ball::bounce() {
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

any Bounce::benchmark() {
    shared_ptr<Random> random = make_shared<Random>();
    int ballCount = 100;
    int bounces = 0;
    Bounce::Ball *balls = new Bounce::Ball[ballCount];
    for(int i = 0; i < ballCount; i++) {
        balls[i] = Bounce::Ball(random);
    }

    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < ballCount; j++) {
            if (balls[j].bounce()) {
                bounces += 1;
            }
        }
    }
    delete[] balls;
    return bounces;
}

bool Bounce::verifyResult(any result) {
    int result_cast = any_cast<int>(result);
    return 1331 == result_cast;
}