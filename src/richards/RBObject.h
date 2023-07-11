
#ifndef RBOBJECT
#define RBOBJECT


#include <memory>

namespace richards {
    class Packet;
    class TaskControlBlock;

    class RBObject {
        public:
            virtual ~RBObject();
            
            std::shared_ptr<Packet> append(std::shared_ptr<Packet> packet, std::shared_ptr<Packet> queueHead);


            static const int IDLER = 0;
            static const int WORKER = 1;
            static const int HANDLER_A = 2;
            static const int HANDLER_B = 3;
            static const int DEVICE_A = 4;
            static const int DEVICE_B = 5;
            static const int NUM_TYPES = 6;

            static const int DEVICE_PACKET_KIND = 0;
            static const int WORK_PACKET_KIND = 1;

            const std::shared_ptr<Packet> NO_WORK = nullptr;
            const std::shared_ptr<TaskControlBlock> NO_TASK = nullptr;
    };
}

#endif //RBOBJECT