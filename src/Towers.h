#ifndef TOWERS
#define TOWERS

#include "Benchmark.h"
#include "som/Error.cpp"

using namespace std;

class Towers : public Benchmark
{
    private:

        class TowersDisk {

            private: 
                int _size;
                shared_ptr<TowersDisk> _next{};

            public:
                TowersDisk(int size);

                int getSize() const;
                shared_ptr<TowersDisk> getNext();
                void setNext(shared_ptr<TowersDisk> value);
        };

        shared_ptr<TowersDisk>* _piles;
        int _movesDone;

        void pushDisk(shared_ptr<TowersDisk> disk, int pile);
        shared_ptr<TowersDisk> popDiskFrom(int pile);
        void moveTopDisk(int fromPile, int toPile);
        void buildTowerAt(int pile, int disks);
        void moveDisks(int disks, int fromPile, int toPile);

    public:
        any benchmark() override;
        bool verifyResult(any result) override;
};

#endif //TOWERS