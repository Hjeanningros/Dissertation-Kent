#ifndef BASICBLOCK
#define BASICBLOCK

#include <vector>
#include <memory>

using namespace std;

namespace havlak {
    class BasicBlock {
        private:
            vector<shared_ptr<BasicBlock>> _inEdges;
            vector<shared_ptr<BasicBlock>> _outEdges;
            int _name;
        
        public:

            BasicBlock(int name);
            vector<shared_ptr<BasicBlock>> getInEdges();
            vector<shared_ptr<BasicBlock>> getOutEdges();
            int getNumPred();
            void addOutEdge(shared_ptr<BasicBlock> to);
            void addInEdge(shared_ptr<BasicBlock> from);
            int customHash();
            
    };
}

#endif //BASICBLOCK