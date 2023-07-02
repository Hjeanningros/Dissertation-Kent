#include "TaskState.cpp"
#include <memory>
#include "Packet.cpp"

namespace richards {
    class TaskControlBlock : public TaskState {
        private: 

            std::shared_ptr<TaskControlBlock> _link 
            int _identity;
            int _priority;
            Packet _packet;
            ProcessFunction _function;
            RBObject handle;

        public:

            TaskControlBlock(TaskControlBlock aLink, int anIdentity, int aPriority, Packet anInitialWorkQueue,
                    TaskState anInitialState, ProcessFunction aBlock, RBObject aPrivateData) {
                _link = aLink;
                _identity = anIdentity;
                _priority = aPriority;
                _input = anInitialWorkQueue;
                setPacketPending(anInitialState.isPacketPending());
                setTaskWaiting(anInitialState.isTaskWaiting());
                setTaskHolding(anInitialState.isTaskHolding());
                _function = aBlock;
                _handle = aPrivateData;
            }

            int getIdentity() { 
                return _identity; 
            }

            TaskControlBlock getLink()  { 
                return _link; 
            }

            int getPriority() { 
                return _priority; 
            }

            TaskControlBlock addInputAndCheckPriority(Packet packet, TaskControlBlock oldTask) {
                if (NO_WORK == _input) {
                    _input = packet;
                    setPacketPending(true);
                    if (priority > oldTask.getPriority()) { 
                        return this; 
                    }
                } else {
                    _input = append(packet, _input);
                }
                return oldTask;
            }

            TaskControlBlock runTask() {
                Packet message;
                if (isWaitingWithPacket()) {
                    message = _input;
                    _input = message.getLink();
                    if (NO_WORK == _input) {
                        running();
                    } else {
                        packetPending();
                    }
                } else {
                    message = NO_WORK;
                }
                return _function.apply(message, handle);
            }
    };
}