#include "ControlFlowGraph.h"

namespace havlak {

    ControlFlowGraph::ControlFlowGraph() {
        _startNode = nullptr;
    }

    shared_ptr<BasicBlock> ControlFlowGraph::createNode(int name) {
        shared_ptr<BasicBlock> node = nullptr;
        if (name < _basicBlockMap.size() && _basicBlockMap[name] != nullptr) {
            node = _basicBlockMap[name];
        } else {
            node = make_shared<BasicBlock>(name);
            if (name >= _basicBlockMap.size()) {
                _basicBlockMap.resize(name + 1, nullptr);
            }
            _basicBlockMap[name] = node;
        }

        if (getNumNodes() == 1) {
            _startNode = node;
        }
        return node;
    }

    void ControlFlowGraph::addEdge(shared_ptr<BasicBlockEdge> edge) {
        _edgeList.push_back(edge);
    }

    int ControlFlowGraph::getNumNodes() {
        return _basicBlockMap.size();
    }

    shared_ptr<BasicBlock> ControlFlowGraph::getStartBasicBlock() {
        return _startNode;
    }

    vector<shared_ptr<BasicBlock>> ControlFlowGraph::getBasicBlocks() {
        return _basicBlockMap;
    }


}