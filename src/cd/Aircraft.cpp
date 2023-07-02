#include "CallSign.cpp"
#include "Vector3D.cpp"
#include <memory>

namespace CD {
    class Aircraft{
        public:
            CallSign _callsign;
            Vector3D _position;

        Aircraft(const CallSign& callsign, const Vector3D& position) {
            _callsign = callsign;
            _position = position;
        }
    };
};