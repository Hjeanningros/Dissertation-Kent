#include <vector>
#include "SimpleLoop.h"
#include <memory>
#include <iostream>
using namespace std;

namespace havlak {
    class UnionFindNode : public enable_shared_from_this<UnionFindNode> {
        private:
            shared_ptr<UnionFindNode> _parent;
            shared_ptr<BasicBlock> _bb;
            shared_ptr<SimpleLoop> _loop;
            int _dfsNumber;

            public:
                UnionFindNode() { } 

                // Initialize this node.
                void initNode(shared_ptr<BasicBlock> bb, int dfsNumber) {
                    cout << "initNode" << endl;
                    _parent = shared_from_this();
                    _bb = bb;
                    _dfsNumber = dfsNumber;
                    _loop = nullptr;
                }

                // Union/Find Algorithm - The find routine.
                shared_ptr<UnionFindNode> findSet() {
                    vector<shared_ptr<UnionFindNode>> nodeList;

                    shared_ptr<UnionFindNode> node = shared_from_this();
                    while (node != node->_parent) {
                        if (node->_parent != node->_parent->_parent) {
                            nodeList.push_back(node);
                        }
                        node = node->_parent;
                    }

                    // Path Compression, all nodes' parents point to the 1st level parent.
                    for (shared_ptr<UnionFindNode> iter : nodeList) {
                        iter->unionSet(_parent);
                    }
                    return node;
                }

                // Union/Find Algorithm - The union routine.
                // Trivial. Assigning parent pointer is enough, we rely on path compression.
                void unionSet(shared_ptr<UnionFindNode> basicBlock) {
                    _parent = basicBlock;
                }

                // Getters/Setters
                shared_ptr<BasicBlock> getBb() const {
                    return _bb;
                }

                shared_ptr<SimpleLoop> getLoop() const {
                    return _loop;
                }

                int getDfsNumber() const {
                    return _dfsNumber;
                }

                void setLoop(shared_ptr<SimpleLoop> loop) {
                    _loop = loop;
                }
    };
}