#include "Benchmark.cpp"
#include <memory>

class List : public Benchmark 
{
    private: 

        class Element {

            private : 
                int _val; // Cange by int use to be std::shared_ptr<void> to represent an object but the vaue will always be an int (to confirm)    
                std::shared_ptr<Element> _next;

            public :
                Element(int v) {
                    _val = v;
                }

                int length() {
                    if (_next == NULL)
                        return 1;
                    else 
                        return 1 + _next->length();
                }
                
                ~Element() {
                }

                int getVal() { return _val; }
                void  setVal(int v) { _val = v; }
                std::shared_ptr<Element> getNext() { return _next; }
                void  setNext(std::shared_ptr<Element> e) { _next = e; }
        };

        std::shared_ptr<Element> makeList(int length) {
            if (length == 0) {
                return NULL;
            } else {
                std::shared_ptr<Element> e = std::make_shared<Element>(length); // Length Represente which value
                e->setNext(makeList(length - 1));
                return e;
            }
        }

        bool isShorterThan(std::shared_ptr<Element>x, std::shared_ptr<Element> y) {
            std::shared_ptr<Element> xTail = x;
            std::shared_ptr<Element> yTail = y;

            while (yTail != NULL) {
                if (xTail == NULL)
                    return true;
                xTail = xTail->getNext();
                yTail = yTail->getNext();
            }
            return false;
        }

        std::shared_ptr<Element> tail(std::shared_ptr<Element> x, std::shared_ptr<Element> y, std::shared_ptr<Element> z) {
            if (isShorterThan(y, x)) {
                return tail(tail(x->getNext(), y, z), 
                    tail(y->getNext(), z, x), 
                    tail(z->getNext(), x, y));
            } else {
                return z;
            }
        }

        public :

            int benchmark() override {
                std::shared_ptr<Element> result = tail(makeList(15), makeList(10), makeList(6));
                return result->length();
            }

            bool verifyResult(int result) override {
                return 10 == result;
            }
};

