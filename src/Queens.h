#ifndef QUEENS
#define QUEENS

#include "Benchmark.h"

using namespace std;

class Queens : public Benchmark
{
    private:
        bool *_freeMaxs;
        bool *_freeRows;
        bool *_freeMins;
        int *_queenRows;

        bool queens();
        bool placeQueen(int c);
        bool getRowColumn(int r, int c);
        void setRowColumn(int r, int c, bool v);

    public:
        any benchmark() override;
        bool verifyResult(any result) override;
};

#endif //QUEENS