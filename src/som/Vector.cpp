#ifndef VECTOR
#define VECTOR

#include <iostream>
#include <functional>
#include <algorithm>

using namespace std;

template<typename E>
class Vector {
    private:
        E* storage;
        size_t _capacity;
        size_t _firstIdx;
        size_t _lastIdx;

    public:
        static Vector<E> with(const E& elem) {
            Vector<E> v(1);
            v.append(elem);
            return v;
        }
        
        explicit Vector(size_t size) : _capacity(size), _firstIdx(0), _lastIdx(0) {
            storage = new E[_capacity];
        }

        Vector() : Vector(50) {}

        ~Vector() {
            delete[] storage;
        }

        E atPtr(size_t idx) const {
            if (idx >= _lastIdx - _firstIdx) {
                return nullptr;
            }
            return storage[_firstIdx + idx];
        }

        E at(size_t idx) const {
            if (idx >= _lastIdx - _firstIdx) {
                return E();
            }
            return storage[_firstIdx + idx];
        }

        void atPut(size_t idx, const E& val) {
            if (idx >= _lastIdx - _firstIdx) {
                size_t newLength = _capacity;
                while (newLength <= idx + _firstIdx) {
                    newLength *= 2;
                }
                E* newStorage = new E[newLength];
                for (size_t i = 0; i < _lastIdx; i++) {
                    newStorage[i] = storage[i];
                }
                delete[] storage;
                storage = newStorage;
                _capacity = newLength;
            }
            storage[_firstIdx + idx] = val;
            if (_lastIdx < idx + 1) {
                _lastIdx = idx + 1;
            }
        }
        
        void append(const E& elem) {
            if (_lastIdx >= _capacity) {
                // Need to expand _capacity first
                _capacity *= 2;
                E* newStorage = new E[_capacity];
                for (size_t i = 0; i < _lastIdx; i++) {
                    newStorage[i] = storage[i];
                }
                delete[] storage;
                storage = newStorage;
            }

            storage[_lastIdx] = elem;
            _lastIdx++;
        }


        bool isEmpty() const {
            return _lastIdx == _firstIdx;
        }

        void forEach(const std::function<void(const E&)>& fn) const {
            for (size_t i = _firstIdx; i < _lastIdx; i++) {
                fn(storage[i]);
            }
        }

        bool hasSome(const std::function<bool(const E&)>& fn) const {
            for (size_t i = _firstIdx; i < _lastIdx; i++) {
                if (fn(storage[i])) {
                    return true;
                }
            }
            return false;
        }

        E getOne(const std::function<bool(const E&)>& fn) const {
            for (size_t i = _firstIdx; i < _lastIdx; i++) {
                const E& e = storage[i];
                if (fn(e)) {
                    return e;
                }
            }
            return nullptr; // Return a default-constructed object if not found
        }

        E first() const {
            if (isEmpty()) {
                return nullptr; // Return a default-constructed object for an empty vector
            }
            return storage[_firstIdx];
        }

        E removeFirst() {
            if (isEmpty()) {
                return nullptr; // Return a default-constructed object for an empty vector
            }
            _firstIdx++;
            return storage[_firstIdx - 1];
        }

        bool remove(const E& obj) {
            bool found = false;
            size_t newLast = _firstIdx;
            for (size_t i = _firstIdx; i < _lastIdx; i++) {
                if (storage[i] == obj) {
                    found = true;
                } else {
                    storage[newLast] = storage[i];
                    newLast++;
                }
            }
            _lastIdx = newLast;
            return found;
        }

        void removeAll() {
            _firstIdx = 0;
            _lastIdx = 0;
        }

        size_t size() const {
            return _lastIdx - _firstIdx;
        }

        size_t getCapacity() const {
            return _capacity;
        }

        void sort(const std::function<bool(const E&, const E&)>& c) {
            if (size() > 0) {
                sort(_firstIdx, _lastIdx - 1, c);
            }
        }
};

#endif //VECTOR
