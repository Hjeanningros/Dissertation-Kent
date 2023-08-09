#include "List.h"

List::Element::Element(int v) {
    _val = v;
}

int List::Element::length() {
    if (_next == nullptr)
        return 1;
    else 
        return 1 + _next->length();
}

int List::Element::getVal() const {
    return _val;
}

void List::Element::setVal(int v) {
    _val = v;
}

shared_ptr<List::Element> List::Element::getNext() {
    return _next;
}

void  List::Element::setNext(shared_ptr<List::Element> e) {
    _next = e;
}

shared_ptr<List::Element> List::makeList(int length) {
    if (length == 0) {
            return {};
    } else {
        shared_ptr<List::Element> e = make_shared<List::Element>(length); // Length Represente which value
        e->setNext(makeList(length - 1));
        return e;
    }
}

bool List::isShorterThan(shared_ptr<List::Element> x, shared_ptr<List::Element> y) {
    shared_ptr<List::Element> xTail = x;
    shared_ptr<List::Element> yTail = y;

    while (yTail != nullptr) {
        if (xTail == nullptr)
            return true;
        xTail = xTail->getNext();
        yTail = yTail->getNext();
    }
    return false;
}

shared_ptr<List::Element> List::tail(shared_ptr<List::Element> x, shared_ptr<List::Element> y, shared_ptr<List::Element> z) {
    if (isShorterThan(y, x)) {
        return tail(tail(x->getNext(), y, z),
            tail(y->getNext(), z, x),
            tail(z->getNext(), x, y));
    } else {
            return z;
    }
}

any List::benchmark() {
    shared_ptr<List::Element> result = tail(makeList(15), makeList(10), makeList(6));
    return result->length();
}

bool List::verifyResult(any result) {
    int result_cast = any_cast<int>(result);
    return 10 == result_cast;
}
