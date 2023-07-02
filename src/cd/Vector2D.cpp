#include <memory>

namespace CD {
    class Vector2D {
        private: 

            static int compareNumbers(double a, double b) {
                if (a == b) {
                    return 0;
                }
                if (a < b) {
                    return -1;
                }
                if (a > b) {
                    return 1;
                }

                // We say that NaN is smaller than non-NaN.
                if (a == a) {
                    return 1;
                }
                return -1;
            }

        public:
            double _x;
            double _y;

            Vector2D(double x, double y) {
                _x = x;
                _y = y;
            }

            Vector2D plus(Vector2D other) const {
                return {_x + other._x, _y + other._y};
            }

            Vector2D minus(Vector2D other) const {
                return {_x - other._x, _y - other._y};
            }

            int compareTo(Vector2D other) {
                int result = compareNumbers(_x, other._x);

                if (result != 0) {
                    return result;
                }

                return compareNumbers(_y, other._y);
            }
    };
};