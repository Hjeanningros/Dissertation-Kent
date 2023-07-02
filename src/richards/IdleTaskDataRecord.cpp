#include "RBObject.cpp"
#include "Packet.cpp"

namespace richards {
    class IdleTaskDataRecord : public RBObject {
        private: 

            int _control;
            int _count;

            int getControl() { 
                return _control; 
            }

            void setControl(int aNumber) {
                _control = aNumber;
            }

            int getCount() { 
                return _count; 
            }
        
            void setCount(int aCount) { 
                _count = aCount; 
            }

            IdleTaskDataRecord() {
                _control = 1;
                _count = 10000;
            }
    };
}