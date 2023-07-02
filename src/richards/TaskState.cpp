#include "RBObject.cpp"
#include "Packet.cpp"

namespace richards {
    class TaskState : public RBObject {
        private: 

            bool _packetPending;
            bool _taskWaiting;
            bool _taskHolding;

        public: 
        
            bool isPacketPending() { 
                return _packetPending; 
            }

            bool isTaskHolding() { 
                return _taskHolding;   
            }
            bool isTaskWaiting() { 
                return _taskWaiting;   
            }

            void setTaskHolding(bool b) { 
                _taskHolding = b; 
            }
            void setTaskWaiting(bool b) { 
                _taskWaiting = b; 
            }

            void setPacketPending(bool b) { 
                _packetPending = b; 
            }

            void packetPending() {
                _packetPending = true;
                _taskWaiting = false;
                _taskHolding = false;
            }

            void running() {
                _packetPending = _taskWaiting = _taskHolding = false;
            }

        void waiting() {
            _packetPending = _taskHolding = false;
            _taskWaiting = true;
        }

        void waitingWithPacket() {
            _taskHolding = false;
            _taskWaiting = _packetPending = true;
        }

        bool isTaskHoldingOrWaiting() {
            return _taskHolding || (!_packetPending && _taskWaiting);
        }

        bool isWaitingWithPacket() {
            return _packetPending && _taskWaiting && !_taskHolding;
        }

        static TaskState createRunning() {
            TaskState t = TaskState();
            t.running();
            return t;
        }

        static TaskState createWaiting() {
            TaskState t = TaskState();
            t.waiting();
            return t;
        }

        static TaskState createWaitingWithPacket() {
            TaskState t = TaskState();
            t.waitingWithPacket();
            return t;
        }
    };
}