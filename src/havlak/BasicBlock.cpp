#include "BasicBlock.h"

namespace havlak {
    
    BasicBlock::BasicBlock(int name) {
        _name = name;
    }

    vector<shared_ptr<BasicBlock>> BasicBlock::getInEdges() {
        return _inEdges;
    }

    vector<shared_ptr<BasicBlock>> BasicBlock::getOutEdges() {
        return _outEdges;
    }

    int BasicBlock::getNumPred() {
        return _inEdges.size();
    }

    void BasicBlock::addOutEdge(shared_ptr<BasicBlock> to) {
        _outEdges.push_back(to);
    }

    void BasicBlock::addInEdge(shared_ptr<BasicBlock> from) {
        _inEdges.push_back(from);
    }

    int BasicBlock::customHash() {
        return _name;
    }
}