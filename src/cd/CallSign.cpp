#include <memory>

namespace CD {
    class CallSign {
        private:
            int _value; 

        public:

            CallSign() = default;

            CallSign(int value) {
                _value = value;
            }

            int compareTo(CallSign& other) const {
                return (_value == other._value) ? 0 : ((_value < other._value) ? -1 : 1);
            }

            CallSign& operator=(const CallSign& C)
            {
                _value = C._value;
                return *this;
            }
    };
};