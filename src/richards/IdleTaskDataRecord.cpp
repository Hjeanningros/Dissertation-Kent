#include "RBObject.h"

namespace richards {
    class IdleTaskDataRecord : public RBObject {
        private: 

            int _control;
            int _count;

        public:
            IdleTaskDataRecord() {
                _control = 1;
                _count = 10000;
            }
            
            int getControl() const {
                return _control; 
            }

            void setControl(int aNumber) {
                _control = aNumber;
            }

            int getCount() const {
                return _count; 
            }
        
            void setCount(int aCount) { 
                _count = aCount; 
            }
    };
}