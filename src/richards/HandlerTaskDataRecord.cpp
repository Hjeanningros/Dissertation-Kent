#include "Packet.h"
#include <memory>
#include <utility>

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
                _deviceIn = std::move(aPacket);
            }

            void deviceInAdd(std::shared_ptr<Packet> packet) {
                _deviceIn = append(std::move(packet), _deviceIn);
            }

            std::shared_ptr<Packet> workIn() { 
                return _workIn; 
            }
            
            void workIn(std::shared_ptr<Packet> aWorkQueue) { 
                _workIn = std::move(aWorkQueue);
            }

            void workInAdd(std::shared_ptr<Packet> packet) {
                _workIn = append(std::move(packet), _workIn);
            }
    };
}