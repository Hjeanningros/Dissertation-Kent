#include "Packet.h"
#include <memory>

namespace richards {
    class HandlerTaskDataRecord : public RBObject {
        private:

            std::shared_ptr<Packet> _workIn;
            std::shared_ptr<Packet> _deviceIn;

        public:

            HandlerTaskDataRecord() {
                _workIn = NO_WORK;
                _deviceIn = NO_WORK;
            }

            std::shared_ptr<Packet> deviceIn() { 
                return _deviceIn; 
            }

            void deviceIn(std::shared_ptr<Packet> aPacket) { 
                _deviceIn = aPacket; 
            }

            void deviceInAdd(std::shared_ptr<Packet> packet) {
                _deviceIn = append(packet, _deviceIn);
            }

            std::shared_ptr<Packet> workIn() { 
                return _workIn; 
            }
            
            void workIn(std::shared_ptr<Packet> aWorkQueue) { 
                _workIn = aWorkQueue; 
            }

            void workInAdd(std::shared_ptr<Packet> packet) {
                _workIn = append(packet, _workIn);
            }
    };
}