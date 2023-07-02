#include <cmath>
#include <vector>
#include "CallSign.cpp"
#include "Aircraft.cpp"

namespace CD {
    class Simulator {
        private :
            std::vector<CallSign> _aircraft;

            public:

            Simulator(int numAircraft) {
                _aircraft = std::vector<CallSign>();

                for (int i = 0; i < numAircraft; i++) {
                    _aircraft.push_back(CallSign(i));
                }
            }

            std::vector<Aircraft> simulate(double time) {
                std::vector<Aircraft> frame = std::vector<Aircraft>();

                for (int i = 0; i < _aircraft.size(); i+=2) {
                    frame.push_back(Aircraft(_aircraft[i], Vector3D(time, cos(time) * 2 + i * 3, 10)));
                    frame.push_back(Aircraft(_aircraft[i + 1], Vector3D(time, cos(time) * 2 + i * 3, 10)));
                }
                return frame;
            }
    };
};
