#include "Permute.h"

void Permute::swap(int i, int j) {
    int tmp = _v[i];
    _v[i] = _v[j];
    _v[j] = tmp;
}

any Permute::benchmark()  {
    _count = 0;
    _v = new int[6];
    permute(6);
    delete[] _v;
    return _count;
}

void Permute::permute(int n) {
    _count++;
    if (n != 0) {
        int n1 = n - 1;
        permute(n1);
        for (int i = n1; i >= 0; i--) {
            swap(n1, i);
            permute(n1);
            swap(n1, i);
        }
    }
}
    
bool Permute::verifyResult(any result)  {
    int result_cast = any_cast<int>(result);
    return result_cast == 8660;
}