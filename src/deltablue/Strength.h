#ifndef STRENGTH
#define STRENGTH

#include <map>
#include <memory>

using namespace std;

namespace deltablue {
    class Strength {
        public:
            class Sym {
                private:
                    int _hash;

                public:
                    Sym(int hash);

                    int customHash() const;
            };

            static shared_ptr<Sym> ABSOLUTE_STRONGEST;
            static shared_ptr<Sym> REQUIRED;
            static shared_ptr<Sym> STRONG_PREFERRED;
            static shared_ptr<Sym> PREFERRED;
            static shared_ptr<Sym> STRONG_DEFAULT;
            static shared_ptr<Sym> DEFAULT;
            static shared_ptr<Sym> WEAK_DEFAULT;
            static shared_ptr<Sym> ABSOLUTE_WEAKEST;

        private:

            int _arithmeticValue;
            shared_ptr<Sym> _symbolicValue;


        public:
            Strength(shared_ptr<Sym> symbolicValue);

            bool sameAs(shared_ptr<Strength> s) const;
            bool stronger(shared_ptr<Strength> s) const;
            bool weaker(shared_ptr<Strength> s) const;
            shared_ptr<Strength> strongest(shared_ptr<Strength> s) const;
            shared_ptr<Strength> weakest(shared_ptr<Strength> s) const;
            int get_arithmeticValue() const;
            static shared_ptr<Strength> of(shared_ptr<Sym> strength);
            static shared_ptr<Strength> absoluteWeakest();
            static shared_ptr<Strength> required();

        private:

            static shared_ptr<Strength> _absoluteWeakest;
            static shared_ptr<Strength> _required;

            static map<shared_ptr<Sym>, int> _strengthTable;
            static map<shared_ptr<Sym>, shared_ptr<Strength>> _strengthConstant;

    };



}

#endif //STRENGTHPLAN