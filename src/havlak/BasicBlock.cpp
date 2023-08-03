#include "BasicBlock.h"

namespace havlak {
    
    BasicBlock::BasicBlock(int name) {
        _name = name;
        _inEdges = make_shared<Vector<shared_ptr<BasicBlock>>>();
        _outEdges = make_shared<Vector<shared_ptr<BasicBlock>>>() ;
    }

    shared_ptr<Vector<shared_ptr<BasicBlock>>> BasicBlock::getInEdges() {
        return _inEdges;
    }

    shared_ptr<Vector<shared_ptr<BasicBlock>>> BasicBlock::getOutEdges() {
        return _outEdges;
    }

    int BasicBlock::getNumPred() {
        return _inEdges->size();
    }

    void BasicBlock::addOutEdge(shared_ptr<BasicBlock> to) {
        _outEdges->append(to);
    }

    void BasicBlock::addInEdge(shared_ptr<BasicBlock> from) {
        _inEdges->append(from);
    }

    int BasicBlock::customHash() {
        return _name;
    }
}