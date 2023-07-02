#include "RBObject.cpp"
#include "Packet.cpp"

namespace richards {
    class DeviceTaskDataRecord :public RBObject {
        private:
            Packet _pending;

        public:
            DeviceTaskDataRecord() {
                _pending = NO_WORK;
            }

            Packet getPending() { 
                return _pending; 
            }
            
            void setPending(Packet packet) { 
                _pending = packet; 
            }
    };
}