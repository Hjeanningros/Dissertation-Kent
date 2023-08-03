#ifndef BASICBLOCK
#define BASICBLOCK

#include "../som/Vector.cpp"
#include <memory>

using namespace std;

namespace havlak {
    class BasicBlock {
        private:
            shared_ptr<Vector<shared_ptr<BasicBlock>>> _inEdges;
            shared_ptr<Vector<shared_ptr<BasicBlock>>> _outEdges;
            int _name;
        
        public:

            BasicBlock(int name);
            shared_ptr<Vector<shared_ptr<BasicBlock>>> getInEdges();
            shared_ptr<Vector<shared_ptr<BasicBlock>>> getOutEdges();
            int getNumPred();
            void addOutEdge(shared_ptr<BasicBlock> to);
            void addInEdge(shared_ptr<BasicBlock> from);
            int customHash();
            
    };
}

#endif //BASICBLOCK