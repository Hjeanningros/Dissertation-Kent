#include "RBObject.cpp"
#include "Packet.cpp"

namespace richards {
    class HandlerTaskDataRecord : public RBObject {
        private:

            Packet _workIn;
            Packet _deviceIn

        public:

            HandlerTaskDataRecord() {
                _workIn = NO_WORK;
                _deviceIn = NO_WORK;
            }

            Packet deviceIn() { 
                return _deviceIn; 
            }

            void deviceIn(Packet& aPacket) { 
                _deviceIn = aPacket; 
            }

            void deviceInAdd(Packet& packet) {
                _deviceIn = append(packet, _deviceIn);
            }

            Packet workIn() { 
                return workIn; 
            }
            
            void workIn(Packet& aWorkQueue) { 
                _workIn = aWorkQueue; 
            }

            void workInAdd(Packet& packet) {
                _workIn = append(packet, _workIn);
            }
    }
}