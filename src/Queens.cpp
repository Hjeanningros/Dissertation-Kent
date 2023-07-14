#include <vector>
#include "Benchmark.cpp"
#include <iostream>

using namespace std;

class Queens : public Benchmark
{
    private:
        vector<bool> _freeMaxs;
        vector<bool> _freeRows;
        vector<bool> _freeMins;
        vector<int> _queenRows;

        bool queens() {
            _freeRows.assign(8, true);

            //for (int i = 0; i < 8; i++)
            //    cout << i << _freeMaxs[i] << endl;
            _freeMaxs.assign(16, true);
            _freeMins.assign(16, true);
            _queenRows.assign(8, -1);
            return placeQueen(0);
        }

        bool placeQueen(int c) {
            for (int r = 0; r < 8; r++) {
                if (getRowColumn(r, c)) {
                    _queenRows[r] = c;
                    setRowColumn(r, c, false);

                    if (c == 7) {
                        return true;
                    }

                    if (placeQueen(c + 1)) {
                        return true;
                    }
                    setRowColumn(r, c, true);
                }
            }
            return false;
        }

        bool getRowColumn(int r, int c) {
            return _freeRows[r] && _freeMaxs[c + r] && _freeMins[c - r + 7];
        }

        void setRowColumn(int r, int c, bool v) {
            _freeRows[r] = v;
            _freeMaxs[c + r] = v;
            _freeMins[c - r + 7] = v;
        }


    public:
        any benchmark() override {
            bool result = true;
            for (int i = 0; i < 10; i++) {
                result = result && queens();
            }
            return result;
        }
    
        bool verifyResult(any result) override {
            bool result_cast = any_cast<bool>(result);
            return result_cast;
        }
};