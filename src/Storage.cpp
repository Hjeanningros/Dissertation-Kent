#include "Storage.h"

void* Storage::buildTreeDepth(int depth, shared_ptr<Random> random) {
    _count++;
    if (depth == 1) {
        return new int[random->next() % 10 + 1];
    } else {
        void* arr[4];
        for (int i = 0; i < 4; i++) {
            arr[i] = buildTreeDepth(depth - 1, random);
        }
        return arr;
    }
}

any Storage::benchmark() {
    shared_ptr<Random> random = make_shared<Random>();
    _count = 0;
    buildTreeDepth(7, random);
    return _count;
}

bool Storage::verifyResult(any result) {
    int result_cast = any_cast<int>(result);
    return 5461 == result_cast;
}