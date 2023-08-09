#include "Towers.h"

using namespace std;

Towers::TowersDisk::TowersDisk(int size) {
    _size = size;
}

int Towers::TowersDisk::getSize() const {
    return _size;
}

shared_ptr<Towers::TowersDisk> Towers::TowersDisk::getNext() {
    return _next;
}

void Towers::TowersDisk::setNext(shared_ptr<TowersDisk> value) {
    _next = move(value);
}

void Towers::pushDisk(shared_ptr<Towers::TowersDisk> disk, int pile) {
    shared_ptr<Towers::TowersDisk> top = _piles[pile];

    if (!(top == nullptr) && (disk->getSize() >= top->getSize())) {
        throw Error("Cannot put a big disk on a smaller one");
    }

    disk->setNext(top);
    _piles[pile] = disk;
}

shared_ptr<Towers::TowersDisk> Towers::popDiskFrom(int pile) {
    shared_ptr<Towers::TowersDisk> top = _piles[pile];

    if (top == nullptr) {
        throw Error("Attempting to remove a disk from an empty pile");
    }

    _piles[pile] = top->getNext();
    top->setNext(nullptr);
    return top;
}

void Towers::moveTopDisk(int fromPile, int toPile) {
    pushDisk(popDiskFrom(fromPile), toPile);
    _movesDone++;
}

void Towers::buildTowerAt(int pile, int disks) {
    for (int i = disks; i >= 0; i--) {
        pushDisk(make_shared<Towers::TowersDisk>(i), pile);
    }
}

void Towers::moveDisks(int disks, int fromPile, int toPile) {
    if(disks == 1) {
        moveTopDisk(fromPile, toPile);
    } else {
        int otherPile = (3 - fromPile) - toPile;
        moveDisks(disks - 1, fromPile, otherPile);
        moveTopDisk(fromPile, toPile);
        moveDisks(disks - 1, otherPile, toPile);
    }
}

any Towers::benchmark() {
    _piles = new shared_ptr<Towers::TowersDisk>[3];
    buildTowerAt(0, 13);
    _movesDone = 0;
    moveDisks(13, 0 ,1);
    delete[] _piles;
    return _movesDone;
}

bool Towers::verifyResult(any result) {
    int result_cast = any_cast<int>(result);
    return 8191 == result_cast;
}