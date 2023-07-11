#include "RBObject.h"
#include <vector>
#include "DeviceTaskDataRecord.cpp"
#include "TaskControlBlock.cpp"
#include "HandlerTaskDataRecord.cpp"
#include <functional>
#include <iostream>
#include "../som/Error.cpp"
#include "IdleTaskDataRecord.cpp"
#include "WorkerTaskDataRecord.cpp"

namespace richards {
    class Scheduler: public RBObject {
        private:

            std::shared_ptr<TaskControlBlock> _taskList;
            std::shared_ptr<TaskControlBlock> _currentTask;
            int _currentTaskIdentity;
            std::vector<std::shared_ptr<TaskControlBlock>> _taskTable;
            int _queuePacketCount;
            int _holdCount;
            int _layout;
            static const bool TRACING = false;

        public:

        Scheduler() {
            // init tracing
            _layout  = 0;

            // init scheduler
            _queuePacketCount = 0;
            _holdCount = 0;
            _taskTable = std::vector<std::shared_ptr<TaskControlBlock>>(NUM_TYPES);

            for (int i = 0; i < NUM_TYPES; i++) {
                _taskTable[i] = NO_TASK;
            }
            _taskList = NO_TASK;
        }


        void createDevice(int identity, int priority, std::shared_ptr<Packet> workPacket, std::shared_ptr<TaskState> state) {
            std::shared_ptr<DeviceTaskDataRecord> data = std::make_shared<DeviceTaskDataRecord>();

            createTask(identity, priority, workPacket, state,
                        [&](std::shared_ptr<Packet> workArg, std::shared_ptr<RBObject> wordArg) -> std::shared_ptr<TaskControlBlock> {
                std::shared_ptr<DeviceTaskDataRecord> dataRecord = std::dynamic_pointer_cast<DeviceTaskDataRecord>(wordArg);
                std::shared_ptr<Packet> functionWork = workArg;
                if (NO_WORK == functionWork) {
                    if (NO_WORK == (functionWork = dataRecord->getPending())) {
                        return markWaiting();
                    } else {
                        dataRecord->setPending(NO_WORK);
                        return queuePacket(workPacket);
                    }
                } else {
                    dataRecord->setPending(functionWork);
                    if (TRACING) {
                        trace(functionWork->getDatum());
                    }
                    return holdSelf();
                }
            }, data);
        }

        void createHandler(int identity, int priority,
            std::shared_ptr<Packet> workPaket, std::shared_ptr<TaskState> state) {

            std::shared_ptr<HandlerTaskDataRecord> data = std::make_shared<HandlerTaskDataRecord>();

            createTask(identity, priority, workPaket, state,
                        [&](std::shared_ptr<Packet> work, std::shared_ptr<RBObject> word) -> std::shared_ptr<TaskControlBlock> {
                std::shared_ptr<HandlerTaskDataRecord> dataRecord = std::dynamic_pointer_cast<HandlerTaskDataRecord>(word);
                if (NO_WORK != work) {
                    if (WORK_PACKET_KIND == work->getKind()) {
                    dataRecord->workInAdd(work);
                    } else {
                    dataRecord->deviceInAdd(work);
                    }
                }

                std::shared_ptr<Packet> workPacket;
                if (NO_WORK == (workPacket = dataRecord->workIn())) {
                    return markWaiting();
                } else {
                    int count = workPacket->getDatum();
                    if (count >= Packet::DATA_SIZE) {
                        dataRecord->workIn(workPacket->getLink());
                        return queuePacket(workPacket);
                    } else {
                        std::shared_ptr<Packet> devicePacket;
                        if (NO_WORK == (devicePacket = dataRecord->deviceIn())) {
                            return markWaiting();
                        } else {
                            dataRecord->deviceIn(devicePacket->getLink());
                            devicePacket->setDatum(workPacket->getData()[count]);
                            workPacket->setDatum(count + 1);
                            return queuePacket(devicePacket);
                        }
                    }
                }
            }, data);
        }

        void createIdler(int identity, int priority, std::shared_ptr<Packet> work,
            std::shared_ptr<TaskState> state) {

                std::shared_ptr<IdleTaskDataRecord> data = std::make_shared<IdleTaskDataRecord>();

                createTask(identity, priority, work, state,
                            [&](std::shared_ptr<Packet> workArg, std::shared_ptr<RBObject> wordArg) -> std::shared_ptr<TaskControlBlock> {
                    std::shared_ptr<IdleTaskDataRecord> dataRecord = std::dynamic_pointer_cast<IdleTaskDataRecord>(wordArg);
                    dataRecord->setCount(dataRecord->getCount() - 1);
                    if (0 == dataRecord->getCount()) {
                        return holdSelf();
                    } else {
                        if (0 == (dataRecord->getControl() & 1)) {
                            dataRecord->setControl(dataRecord->getControl() / 2);
                            return release(DEVICE_A);
                        } else {
                            dataRecord->setControl((dataRecord->getControl() / 2) ^ 53256);
                            return release(DEVICE_B);
                        }
                    }
                    }, data);
        }

        std::shared_ptr<Packet> createPacket(std::shared_ptr<Packet> link, int identity, int kind) {
            return std::make_shared<Packet>(link, identity, kind);
        }

        void createTask(int identity, int priority,
            std::shared_ptr<Packet> work, std::shared_ptr<TaskState> state,
            std::function<std::shared_ptr<TaskControlBlock>(std::shared_ptr<Packet> work, std::shared_ptr<RBObject> word)> aBlock, 
            std::shared_ptr<RBObject> data) {

            std::shared_ptr<TaskControlBlock> t = std::make_shared<TaskControlBlock>(_taskList, identity,
                priority, work, state, aBlock, data);
            _taskList = t;
            _taskTable[identity] = t;
        }

        void createWorker(int identity, int priority,
            std::shared_ptr<Packet> workPaket, std::shared_ptr<TaskState> state) {

            std::shared_ptr<WorkerTaskDataRecord> dataRecord = std::make_shared<WorkerTaskDataRecord>();

            createTask(identity, priority, workPaket, state,
                [&](std::shared_ptr<Packet> work, std::shared_ptr<RBObject> word) -> std::shared_ptr<TaskControlBlock> {
                std::shared_ptr<WorkerTaskDataRecord> data = std::dynamic_pointer_cast<WorkerTaskDataRecord>(word);
                if (NO_WORK == work) {
                    return markWaiting();
                } else {
                    data->setDestination((HANDLER_A == data->getDestination()) ? HANDLER_B : HANDLER_A);
                    work->setIdentity(data->getDestination());
                    work->setDatum(0);
                    for (int i = 0; i < Packet::DATA_SIZE; i++) {
                        data->setCount(data->getCount() + 1);
                        if (data->getCount() > 26) { 
                            data->setCount(1); 
                        }
                        work->getData()[i] = 65 + data->getCount() - 1;
                    }
                    return queuePacket(work);
                    }
                }, dataRecord);
        }

        bool start() {
            std::shared_ptr<Packet> workQ;
            createIdler(IDLER, 0, NO_WORK, TaskState::createRunning());
            workQ = createPacket(NO_WORK, WORKER, WORK_PACKET_KIND);
            workQ = createPacket(workQ,   WORKER, WORK_PACKET_KIND);

            createWorker(WORKER, 1000, workQ, TaskState::createWaitingWithPacket());
            workQ = createPacket(NO_WORK, DEVICE_A, DEVICE_PACKET_KIND);
            workQ = createPacket(workQ,   DEVICE_A, DEVICE_PACKET_KIND);
            workQ = createPacket(workQ,   DEVICE_A, DEVICE_PACKET_KIND);

            createHandler(HANDLER_A, 2000, workQ, TaskState::createWaitingWithPacket());
            workQ = createPacket(NO_WORK, DEVICE_B, DEVICE_PACKET_KIND);
            workQ = createPacket(workQ, DEVICE_B, DEVICE_PACKET_KIND);
            workQ = createPacket(workQ, DEVICE_B, DEVICE_PACKET_KIND);

            createHandler(HANDLER_B, 3000, workQ, TaskState::createWaitingWithPacket());
            createDevice(DEVICE_A, 4000, NO_WORK, TaskState::createWaiting());
            createDevice(DEVICE_B, 5000, NO_WORK, TaskState::createWaiting());

            schedule();

            return _queuePacketCount == 23246 && _holdCount == 9297;
        }

        std::shared_ptr<TaskControlBlock> findTask(int identity) {
            std::shared_ptr<TaskControlBlock> t = _taskTable[identity];
            if (NO_TASK == t) { 
                throw Error("findTask failed");
            }
            return t;
        }

        std::shared_ptr<TaskControlBlock> holdSelf() {
            _holdCount = _holdCount + 1;
            _currentTask->setTaskHolding(true);
            return _currentTask->getLink();
        }

        std::shared_ptr<TaskControlBlock> queuePacket(std::shared_ptr<Packet> packet) {
            std::shared_ptr<TaskControlBlock> t = findTask(packet->getIdentity());
            if (NO_TASK == t) { 
                return NO_TASK; 
            }

            _queuePacketCount = _queuePacketCount + 1;

            packet->setLink(NO_WORK);
            packet->setIdentity(_currentTaskIdentity);
            return t->addInputAndCheckPriority(packet, _currentTask);
        }

        std::shared_ptr<TaskControlBlock> release(int identity) {
            std::shared_ptr<TaskControlBlock> t = findTask(identity);
            if (NO_TASK == t) { 
                return NO_TASK; 
            }
            t->setTaskHolding(false);
            if (t->getPriority() > _currentTask->getPriority()) {
                return t;
            } else {
                return _currentTask;
            }
        }

        void trace(int id) {
            _layout = _layout - 1;
            if (0 >= _layout) {
                std::cout << std::endl;
                _layout = 50;
            }
            std::cout << id <<std::endl;
        }

        std::shared_ptr<TaskControlBlock> markWaiting() {
            _currentTask->setTaskWaiting(true);
            return _currentTask;
        }

        void schedule() {
            _currentTask = _taskList;
            while (NO_TASK != _currentTask) {
                if (_currentTask->isTaskHoldingOrWaiting()) {
                    _currentTask = _currentTask->getLink();
                } else {
                    _currentTaskIdentity = _currentTask->getIdentity();
                    if (TRACING) {
                        trace(_currentTaskIdentity);
                    }

                    _currentTask = _currentTask->runTask();
                }
            }
        }
    };
}