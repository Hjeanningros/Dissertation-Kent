#include "TaskControlBlock.h"

using namespace std;

namespace richards {

    TaskControlBlock::TaskControlBlock(shared_ptr<TaskControlBlock> aLink, int anIdentity, int aPriority, shared_ptr<Packet> anInitialWorkQueue,
            const shared_ptr<TaskState>& anInitialState, function<shared_ptr<TaskControlBlock>(shared_ptr<Packet> work, shared_ptr<RBObject> word)> aBlock,
            shared_ptr<RBObject> aPrivateData) {
        _link = move(aLink);
        _identity = anIdentity;
        _priority = aPriority;
        _input = move(anInitialWorkQueue);
        setPacketPending(anInitialState->isPacketPending());
        setTaskWaiting(anInitialState->isTaskWaiting());
        setTaskHolding(anInitialState->isTaskHolding());
        _function = move(aBlock);
        _handle = move(aPrivateData);
    }

    int TaskControlBlock::getIdentity() const {
        return _identity; 
    }

    shared_ptr<TaskControlBlock> TaskControlBlock::getLink()  { 
        return _link; 
    }

    int TaskControlBlock::getPriority() const {
        return _priority; 
    }

    shared_ptr<TaskControlBlock> TaskControlBlock::addInputAndCheckPriority(shared_ptr<Packet> packet, shared_ptr<TaskControlBlock> oldTask) {
        if (NO_WORK == _input) {
            _input = packet;
            setPacketPending(true);
            if (_priority > oldTask->getPriority()) { 
                return shared_from_this();
            }
        } else {
            _input = append(packet, _input);
        }
        return oldTask;
    }

    shared_ptr<TaskControlBlock> TaskControlBlock::runTask() {
        shared_ptr<Packet> message;
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
}