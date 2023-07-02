#include <vector>
#include "Benchmark.cpp"

class Queens : public Benchmark
{
    private:
        std::vector<bool> _freeMaxs;
        std::vector<bool> _freeRows;
        std::vector<bool> _freeMins;
        std::vector<int> _queenRows;

        bool queens() {
            _freeMaxs.assign(8, true);
            _freeRows.assign(16, true);
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
        std::any benchmark() override {
            bool result = true;
            for (int i = 0; i < 10; i++) {
                result = result && queens();
            }
            return result;
        }
    
        bool verifyResult(std::any result) override {
            bool result_cast = std::any_cast<bool>(result);
            return result_cast;
        }
};