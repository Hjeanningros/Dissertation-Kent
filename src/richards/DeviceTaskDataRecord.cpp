#include "Packet.h"
#include <memory>
#include <utility>

namespace richards {
    class DeviceTaskDataRecord : public RBObject {
        private:
            std::shared_ptr<Packet> _pending;

        public:
            DeviceTaskDataRecord() {
                _pending = NO_WORK;
            }

            std::shared_ptr<Packet> getPending() {
                return _pending; 
            }
            
            void setPending(std::shared_ptr<Packet> packet) {
                _pending = packet;
            }
    };
}