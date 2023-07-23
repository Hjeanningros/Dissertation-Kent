#include "ControlFlowGraph.h"
#include "LoopStructureGraph.h"
#include "HavlakLoopFinder.cpp"
#include <memory>

using namespace std;

namespace havlak {
    class LoopTesterApp {

        private:

            shared_ptr<ControlFlowGraph> _cfg;
            shared_ptr<LoopStructureGraph> _lsg;

            int buildDiamond(int start) {
                int bb0 = start;
                BasicBlockEdge(_cfg, bb0, bb0 + 1);
                BasicBlockEdge(_cfg, bb0, bb0 + 2);
                BasicBlockEdge(_cfg, bb0 + 1, bb0 + 3);
                BasicBlockEdge(_cfg, bb0 + 2, bb0 + 3);

                return bb0 + 3;
            }

            void buildConnect(int start, int end) {
                BasicBlockEdge(_cfg, start, end);
            }

            int buildStraight(int start, int n) {
                for (int i = 0; i < n; i++) {
                    buildConnect(start + i, start + i + 1);
                }
                return start + n;
            }

            int buildBaseLoop(int from) {
                int header = buildStraight(from, 1);
                int diamond1 = buildDiamond(header);
                int d11 = buildStraight(diamond1, 1);
                int diamond2 = buildDiamond(d11);
                int footer = buildStraight(diamond2, 1);
                buildConnect(diamond2, d11);
                buildConnect(diamond1, header);

                buildConnect(footer, from);
                footer = buildStraight(footer, 1);
                return footer;
            }

            void constructSimpleCFG() {
                _cfg->createNode(0);
                buildBaseLoop(0);
                _cfg->createNode(1);
                BasicBlockEdge(_cfg, 0, 2);
            }

        public:
            LoopTesterApp() : _cfg(), _lsg() {
                _cfg->createNode(0);
            }

            vector<int> main(int numDummyLoops, int findLoopIterations, int parLoops,
                    int pparLoops, int ppparLoops) {
                constructSimpleCFG();
                addDummyLoops(numDummyLoops);
                constructCFG(parLoops, pparLoops, ppparLoops);

                findLoops(_lsg);
                for (int i = 0; i < findLoopIterations; i++) {
                    findLoops(make_shared<LoopStructureGraph>());
                }

                _lsg->calculateNestingLevel();
                vector<int> result = {_lsg->getNumLoops(), _cfg->getNumNodes()};
                return result;
            }

            void constructCFG(int parLoops, int pparLoops, int ppparLoops) {
                int n = 2;

                for (int parlooptrees = 0; parlooptrees < parLoops; parlooptrees++) {
                    _cfg->createNode(n + 1);
                    buildConnect(2, n + 1);
                    n += 1;

                    for (int i = 0; i < pparLoops; i++) {
                        int top = n;
                        n = buildStraight(n, 1);
                        for (int j = 0; j < ppparLoops; j++) {
                            n = buildBaseLoop(n);
                        }
                        int bottom = buildStraight(n, 1);
                        buildConnect(n, top);
                        n = bottom;
                    }
                    buildConnect(n, 1);
                }
            }

            void addDummyLoops(int numDummyLoops) {
                for (int dummyloop = 0; dummyloop < numDummyLoops; dummyloop++) {
                    findLoops(_lsg);
                }
            }

            void findLoops(shared_ptr<LoopStructureGraph> loopStructure) {
                HavlakLoopFinder finder(_cfg, loopStructure);
                finder.findLoops();
            }
        
    };
}