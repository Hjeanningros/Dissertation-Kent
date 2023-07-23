#ifndef CONTROLFLOWGRAPH
#define CONTROLFLOWGRAPH

#include <vector>
#include "BasicBlock.h"
#include "BasicBlockEdge.h"

using namespace std;

namespace havlak {
    
    class ControlFlowGraph {
        private:
            vector<shared_ptr<BasicBlock>> _basicBlockMap;
            shared_ptr<BasicBlock> _startNode;
            vector<shared_ptr<BasicBlockEdge>> _edgeList;
        
        public:
            ControlFlowGraph();
            shared_ptr<BasicBlock> createNode(int name);
            void addEdge(shared_ptr<BasicBlockEdge> edge);
            int getNumNodes();
            shared_ptr<BasicBlock> getStartBasicBlock();
            vector<shared_ptr<BasicBlock>> getBasicBlocks();

    };
}

#endif //CONTROLFLOWGRAPH
