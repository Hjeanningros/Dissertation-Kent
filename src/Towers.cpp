#include <vector>
#include "Benchmark.cpp"
#include "som/Error.cpp"

class Towers : public Benchmark
{
    private:

        class TowersDisk {

            private: 
                int _size;
                std::shared_ptr<TowersDisk> _next;

            public: 
                TowersDisk(int size) {
                    _size = size;
                }

                int getSize() {
                    return _size;
                }

                std::shared_ptr<TowersDisk> getNext() {
                    return _next;
                }

                void setNext(std::shared_ptr<TowersDisk> value) {
                    _next = value;
                }
        };

        std::vector<std::shared_ptr<TowersDisk>> _piles;
        int _movesDone;

        void pushDisk(std::shared_ptr<TowersDisk> disk, int pile) {
            std::shared_ptr<TowersDisk> top = _piles[pile];

            if (!(top == NULL) && (disk->getSize() >= top->getSize())) {
                throw Error("Cannot put a big disk on a smaller one");
            }

            disk->setNext(top);
            _piles[pile] = disk;
        }

        std::shared_ptr<TowersDisk> popDiskFrom(int pile) {
            std::shared_ptr<TowersDisk> top = _piles[pile];

            if (top == NULL) {
                throw Error("Attempting to remove a disk from an empty pile");
            }

            _piles[pile] = top->getNext();
            top->setNext(NULL);
            return top;
        }

        void moveTopDisk(int fromPile, int toPile) {
            pushDisk(popDiskFrom(fromPile), toPile);
            _movesDone++;
        }

        void buildTowerAt(int pile, int disks) {
            for (int i = disks; i >= 0; i--) {
                pushDisk(std::make_shared<TowersDisk>(i), pile);
            }
        }

        void moveDisks(int disks, int fromPile, int toPile) {
            if(disks == 1) {
                moveTopDisk(fromPile, toPile);
            } else {
                int otherPile = (3 - fromPile) - toPile;
                moveDisks(disks - 1, fromPile, otherPile);
                moveTopDisk(fromPile, toPile);
                moveDisks(disks - 1, otherPile, toPile);
            }
        }

    public:
        int benchmark() override {
            _piles = std::vector<std::shared_ptr<TowersDisk>>(3);
            buildTowerAt(0, 13);
            _movesDone = 0;
            moveDisks(13, 0 ,1);

            return _movesDone;
        }

        bool verifyResult(int result) override {
            return 8191 == (int)result;
        }
};