#ifndef MOTION
#define MOTION

#include "CallSign.h"
#include "Vector3D.h"
#include "Constants.h"
#include <memory>

using namespace std;

namespace CD {
    class Motion {
        private:
            shared_ptr<Vector3D> delta() const;

        public:
            shared_ptr<CallSign> _callsign;
            shared_ptr<Vector3D> _posOne;
            shared_ptr<Vector3D> _posTwo;

            Motion(shared_ptr<CallSign> callsign, shared_ptr<Vector3D> posOne, shared_ptr<Vector3D> posTwo);

            shared_ptr<Vector3D> findIntersection(shared_ptr<Motion> other);
    };
}

#endif //MOTION