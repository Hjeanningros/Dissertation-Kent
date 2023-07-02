#include "Packet.cpp"

namespace richards {
    class RBObject {
        public:
            Packet append(Packet packet, Packet queueHead) {
                    packet.setLink(NO_WORK);
                    if (NO_WORK == queueHead) {
                        return packet;
                    }

                    Packet mouse = queueHead;
                    Packet link;
                    while (NO_WORK != (link = mouse.getLink())) {
                        mouse = link;
                    }
                    mouse.setLink(packet);
                    return queueHead;
                }

            constexpr static const int IDLER = 0;
            constexpr static const int WORKER = 1;
            constexpr static const int HANDLER_A = 2;
            constexpr static const int HANDLER_B = 3;
            constexpr static const int DEVICE_A = 4;
            constexpr static const int DEVICE_B = 5;
            constexpr static const int NUM_TYPES = 6;

            constexpr static const int DEVICE_PACKET_KIND = 0;
            constexpr static const int WORK_PACKET_KIND = 1;

            constexpr static const Packet NO_WORK = Packet{};
            constexpr static const TaskControlBlock NO_TASK = TaskControlBlock{};
    };
}