#ifndef LIST
#define LIST

#include "Benchmark.h"

using namespace std;

class List : public Benchmark 
{
    private: 
        class Element {

            private : 
                int _val; // Cange by int use to be shared_ptr<void> to represent an object but the vaue will always be an int (to confirm)
                shared_ptr<Element> _next;

            public :
                // Element() = default;

                Element(int v);

                int length();
                int getVal() const;
                void  setVal(int v);
                shared_ptr<Element> getNext();
                void  setNext(shared_ptr<Element> e);
        };

        shared_ptr<Element> makeList(int length);
        static bool isShorterThan(shared_ptr<Element> x, shared_ptr<Element> y);
        shared_ptr<Element> tail(shared_ptr<Element> x, shared_ptr<Element> y, shared_ptr<Element> z);

    public :
        any benchmark() override;
        bool verifyResult(any result) override;
};


#endif //LIST