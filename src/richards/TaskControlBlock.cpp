#include "TaskState.h"
#include <memory>
#include "Packet.h"
#include <functional>
#include <utility>
#include <iostream>

namespace richards {
    class TaskControlBlock : public TaskState {
        private: 

            std::shared_ptr<TaskControlBlock> _link;
            int _identity;
            int _priority;
            std::shared_ptr<Packet> _input;
            std::function<std::shared_ptr<TaskControlBlock>(std::shared_ptr<Packet> work, std::shared_ptr<RBObject> word)> _function;
            std::shared_ptr<RBObject> _handle;

        public:

            explicit TaskControlBlock(TaskControlBlock* task) {
                std::cout << "ici " << std::endl;
                _link = std::move(task->_link);
                _identity = task->_identity;
                _priority = task->_priority;
                _input = std::move(task->_input);
                setPacketPending(task->isPacketPending());
                setTaskWaiting(task->isTaskWaiting());
                setTaskHolding(task->isTaskHolding());
                _function = std::move(task->_function);
                _handle = std::move(task->_handle);
            }

            TaskControlBlock(std::shared_ptr<TaskControlBlock> aLink, int anIdentity, int aPriority, std::shared_ptr<Packet> anInitialWorkQueue,
                    const std::shared_ptr<TaskState>& anInitialState, std::function<std::shared_ptr<TaskControlBlock>(std::shared_ptr<Packet> work, std::shared_ptr<RBObject> word)> aBlock,
                    std::shared_ptr<RBObject> aPrivateData) {
                _link = std::move(aLink);
                _identity = anIdentity;
                _priority = aPriority;
                _input = std::move(anInitialWorkQueue);
                setPacketPending(anInitialState->isPacketPending());
                setTaskWaiting(anInitialState->isTaskWaiting());
                setTaskHolding(anInitialState->isTaskHolding());
                _function = std::move(aBlock);
                _handle = std::move(aPrivateData);
            }

            int getIdentity() const {
                return _identity; 
            }

            std::shared_ptr<TaskControlBlock> getLink()  { 
                return _link; 
            }

            int getPriority() const {
                return _priority; 
            }

            std::shared_ptr<TaskControlBlock> addInputAndCheckPriority(std::shared_ptr<Packet> packet, std::shared_ptr<TaskControlBlock> oldTask) {
                if (NO_WORK == _input) {
                    _input = packet;
                    setPacketPending(true);
                    if (_priority > oldTask->getPriority()) { 
                        return std::make_shared<TaskControlBlock>(this);
                    }
                } else {
                    _input = append(packet, _input);
                }
                return oldTask;
            }

            std::shared_ptr<TaskControlBlock> runTask() {
                std::shared_ptr<Packet> message;
                if (isWaitingWithPacket()) {
                    message = _input;
                    _input = message->getLink();
                    if (NO_WORK == _input) {
                        running();
                    } else {
                        packetPending();
                    }
                } else {
                    message = NO_WORK;
                }
                return _function(message, _handle);
            }
    };

}