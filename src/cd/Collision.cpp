#include "CallSign.cpp"
#include "Vector3D.cpp"
#include <memory>

namespace CD {
    class Collision {
        public:
            CallSign _aircraftA;
            CallSign _aircraftB;
            Vector3D _position;

            Collision(const CallSign& aircraftA, const CallSign& aircraftB, const Vector3D& position) {
                _aircraftA = aircraftA;
                _aircraftB = aircraftB;
                _position = position;
            }
    };
};