#include <cmath>

namespace CD {
    class Vector3D {
        private:
            
        public:
            double _x;
            double _y;
            double _z;
            
            Vector3D() = default;

            Vector3D(double x, double y, double z) {
                _x = x;
                _y = y;
                _z = z;
            }

            Vector3D plus(Vector3D other) const {
                return {_x + other._x, _y + other._y, _z + other._z};
            }

            Vector3D minus(Vector3D other) const {
                return {_x - other._x, _y - other._y, _z - other._z};
            }

            double dot(Vector3D other) const {
                return _x * other._x + _y * other._y + _z * other._z;
            }

            double squaredMagnitude() const {
                return dot(Vector3D(_x, _y, _z));
            }


            double magnitude() const {
                return sqrt(squaredMagnitude());
            }

            Vector3D times(double amount) const {
                return {_x * amount, _y * amount, _z * amount};
            }

            Vector3D& operator=(const Vector3D& C)
            {
                _x = C._x;
                _y = C._y;
                _z = C._z;

                return *this;
            }
    };
};