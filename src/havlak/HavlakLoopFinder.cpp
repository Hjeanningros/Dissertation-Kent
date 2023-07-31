#include "ControlFlowGraph.h"
#include "LoopStructureGraph.h"
#include "UnionFindNode.cpp"
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

namespace havlak {
    class HavlakLoopFinder {
        private:

            enum BasicBlockClass {
                BB_TOP,          // uninitialized
                BB_NONHEADER,    // a regular BB
                BB_REDUCIBLE,    // reducible loop
                BB_SELF,         // single BB loop
                BB_IRREDUCIBLE,  // irreducible loop
                BB_DEAD,         // a dead BB
                BB_LAST          // Sentinel
            };

            shared_ptr<ControlFlowGraph> _cfg;      // Control Flow Graph
            shared_ptr<LoopStructureGraph> _lsg;    // Loop Structure Graph

            static constexpr int UNVISITED = numeric_limits<int>::max();
            static constexpr int MAXNONBACKPREDS = 32 * 1024;

            vector<set<int>> _nonBackPreds;
            vector<vector<int>> _backPreds;
            map<shared_ptr<BasicBlock>, int> _number;
            int _maxSize = 0;
            vector<int> _header;
            vector<BasicBlockClass> _type;
            vector<int> _last;
            vector<shared_ptr<UnionFindNode>> _nodes;

            bool findElement(const std::vector<shared_ptr<UnionFindNode>> nodePool, shared_ptr<UnionFindNode> ydash) {
                return std::find_if(nodePool.begin(), nodePool.end(),
                        [ydash](shared_ptr<UnionFindNode> e) { return e == ydash; }) != nodePool.end();
            }

            bool isAncestor(int w, int v) {
                return w <= v && v <= _last[w];
            }

            int doDFS(shared_ptr<BasicBlock> currentNode, int current) {
                cout << "doDFS" << endl;
                _nodes[current]->initNode(currentNode, current);
                _number.insert(make_pair(currentNode, current));
                cout << "1" << endl;
                int lastId = current;
                vector<shared_ptr<BasicBlock>> outerBlocks = currentNode->getOutEdges();
                cout << "outerBlocks.size() = " << outerBlocks.size() << endl;
                for (int i = 0; i < outerBlocks.size(); i++) {
                    shared_ptr<BasicBlock> target = outerBlocks[i];
                    if (_number[target] == UNVISITED) {
                        cout << "in IF target = " << target << "lastId + 1 == " << (lastId + 1) << endl;
                        lastId = doDFS(target, lastId + 1);
                    }
                }

                _last[current] = lastId;
                cout << "end of doDFS" << endl;
                return lastId;
            }

            void initAllNodes() {
                cout << "initAllNodes size = " <<  _cfg->getBasicBlocks().size() << endl;

                for (shared_ptr<BasicBlock> bb: _cfg->getBasicBlocks())
                    _number.insert(make_pair(bb, UNVISITED));

                doDFS(_cfg->getStartBasicBlock(), 0);
            }

            void identifyEdges(int size) {
                for (int w = 0; w < size; w++) {
                    _header[w] = 0;
                    _type[w] = BB_NONHEADER;

                    shared_ptr<BasicBlock> nodeW = _nodes[w]->getBb();
                    if (nodeW == nullptr) {
                        _type[w] = BB_DEAD;
                    } else {
                        processEdges(nodeW, w);
                    }
                }
            }

            void processEdges(shared_ptr<BasicBlock> nodeW, int w) {
                if (nodeW->getNumPred() > 0) {
                    for (shared_ptr<BasicBlock> nodeV : nodeW->getInEdges()) {
                        int v = _number[nodeV];
                        if (v != UNVISITED) {
                            if (isAncestor(w, v)) {
                                _backPreds[w].push_back(v);
                            } else {
                                _nonBackPreds[w].insert(v);
                            }
                        }
                    }
                }
            }

            void stepEProcessNonBackPreds(int w, vector<shared_ptr<UnionFindNode>> nodePool, vector<shared_ptr<UnionFindNode>> workList, shared_ptr<UnionFindNode> x) {
                for (int iter : _nonBackPreds[x->getDfsNumber()]) {
                  shared_ptr<UnionFindNode> y = _nodes[iter];
                  shared_ptr<UnionFindNode> ydash = y->findSet();

                  if (!isAncestor(w, ydash->getDfsNumber())) {
                      _type[w] = BB_IRREDUCIBLE;
                      _nonBackPreds[w].insert(ydash->getDfsNumber());
                  } else {
                      if (ydash->getDfsNumber() != w) {
                          if (!findElement(nodePool, ydash)) {
                              workList.push_back(ydash);
                              nodePool.push_back(ydash);
                          }
                      }
                  }
                }
            }


            void setLoopAttributes(int w, vector<shared_ptr<UnionFindNode>> nodePool, shared_ptr<SimpleLoop> loop) {
                _nodes[w]->setLoop(loop);

                for (shared_ptr<UnionFindNode> node: nodePool) {
                    _header[node->getDfsNumber()] = w;
                    node->unionSet(_nodes[w]);

                    // Nested loops are not added, but linked together.
                    if (node->getLoop() != nullptr) {
                        node->getLoop()->setParent(loop);
                    } else {
                        loop->addNode(node->getBb());
                    }
                }
            }

            void stepD(int w, vector<shared_ptr<UnionFindNode>> nodePool) {
                for (int v : _backPreds[w]) {
                    if (v != w) {
                        nodePool.push_back(_nodes[v]->findSet());
                    } else {
                        _type[w] = BB_SELF;
                    }   
                }
            }



        public:
            HavlakLoopFinder(shared_ptr<ControlFlowGraph> cfg, shared_ptr<LoopStructureGraph> lsg) {
                _cfg = cfg;
                _lsg = lsg;
            }

            void findLoops() {
                cout << "FindLoop" << endl;
                if (_cfg->getStartBasicBlock() == nullptr) {
                    return;
                }
                cout << "Step1" << endl;

                int size = _cfg->getNumNodes();

                cout << "Step2" << endl;
                _nonBackPreds.clear();
                _backPreds.clear();
                _number.clear();
                if (size > _maxSize) {
                    _header = vector<int>(size);
                    _type = vector<BasicBlockClass>(size);
                    _last = vector<int>(size);
                    _nodes = vector<shared_ptr<UnionFindNode>>(size);
                    _maxSize = size;
                }
                cout << "Step3" << endl;

                for (int i = 0; i < size; ++i) {
                    _nodes[i] = make_shared<UnionFindNode>();
                }
                cout << "Step4" << endl;

                initAllNodes();
                cout << "Step5" << endl;
                identifyEdges(size);
                cout << "Step6" << endl;

                // Start node is root of all other loops.
                _header[0] = 0;

                for (int w = size - 1; w >= 0; w--) {
                    // this is 'P' in Havlak's paper
                    vector<shared_ptr<UnionFindNode>> nodePool = vector<shared_ptr<UnionFindNode>>();

                    shared_ptr<BasicBlock> nodeW = _nodes[w]->getBb();
                    if (nodeW != nullptr) {
                    stepD(w, nodePool);

                    // Copy nodePool to workList.
                    //
                    vector<shared_ptr<UnionFindNode>> workList = vector<shared_ptr<UnionFindNode>>();
                    for (shared_ptr<UnionFindNode> niter: nodePool) {
                        workList.push_back(niter);
                    }

                    if (nodePool.size() != 0) {
                      _type[w] = BB_REDUCIBLE;
                    }

                    // work the list...
                    //
                    while (!workList.empty()) {
                        shared_ptr<UnionFindNode> x = workList.front();
                        workList.erase(workList.begin());

                        int nonBackSize = _nonBackPreds[x->getDfsNumber()].size();
                        if (nonBackSize > MAXNONBACKPREDS) {
                            return;
                          }
                        stepEProcessNonBackPreds(w, nodePool, workList, x);
                    }


                    if ((nodePool.size() > 0) || (_type[w] == BB_SELF)) {
                      shared_ptr<SimpleLoop> loop = _lsg->createNewLoop(nodeW, _type[w] != BB_IRREDUCIBLE);
                      setLoopAttributes(w, nodePool, loop);
                    }
                  }
                }  // Step c
              }  // findLoops
    };
}