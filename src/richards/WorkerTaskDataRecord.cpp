#include "RBObject.cpp"
#include "Packet.cpp"

namespace richards {
    class WorkerTaskDataRecord : public  RBObject {
        private:
            int _destination;
            int _count;

        public:
            WorkerT askDataRecord() {
                _destination = HANDLER_A;
                _count = 0;
            }
                
            int getCount() { 
                return _count; 
            }

            void setCount(final int aCount) { 
                _count = aCount; 
            }

            int getDestination() { 
                return _destination; 
            }

            void setDestination(int aHandler) { 
                _destination = aHandler; 
            }
    };
}