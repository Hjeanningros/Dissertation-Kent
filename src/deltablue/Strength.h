#ifndef STRENGTH
#define STRENGTH

#include <unordered_map>
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

            const shared_ptr<Sym> ABSOLUTE_STRONGEST = make_shared<Sym>(0);
            const shared_ptr<Sym> REQUIRED           = make_shared<Sym>(1);
            const shared_ptr<Sym> STRONG_PREFERRED   = make_shared<Sym>(2);
            const shared_ptr<Sym> PREFERRED          = make_shared<Sym>(3);
            const shared_ptr<Sym> STRONG_DEFAULT     = make_shared<Sym>(4);
            const shared_ptr<Sym> DEFAULT            = make_shared<Sym>(5);
            const shared_ptr<Sym> WEAK_DEFAULT       = make_shared<Sym>(6);
            const shared_ptr<Sym> ABSOLUTE_WEAKEST   = make_shared<Sym>(7);

        private:
            static unordered_map<Sym, int> _strengthTable;
            static unordered_map<Sym, shared_ptr<Strength>> _strengthConstant;

            int _arithmeticValue;
            shared_ptr<Sym> _symbolicValue;


        public:
            Strength(shared_ptr<Sym> symbolicValue);

            shared_ptr<Strength> _absoluteWeakest;
            shared_ptr<Strength> _required;

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

            std::unordered_map<shared_ptr<Sym>, int> strengthTable = {
                {make_shared<Sym>(0), -10000},
                {make_shared<Sym>(1), -800},
                {make_shared<Sym>(2), -600},
                {make_shared<Sym>(3), -400},
                {make_shared<Sym>(4), -200},
                {make_shared<Sym>(5), 0},
                {make_shared<Sym>(6), 500},
                {make_shared<Sym>(7), 10000}
            };


            std::unordered_map<shared_ptr<Sym>, shared_ptr<Strength>> strengthConstant = {
                {make_shared<Sym>(0), make_shared<Strength>(make_shared<Sym>(0))},
                {make_shared<Sym>(1), make_shared<Strength>(make_shared<Sym>(1))},
                {make_shared<Sym>(2), make_shared<Strength>(make_shared<Sym>(2))},
                {make_shared<Sym>(3), make_shared<Strength>(make_shared<Sym>(3))},
                {make_shared<Sym>(4), make_shared<Strength>(make_shared<Sym>(4))},
                {make_shared<Sym>(5), make_shared<Strength>(make_shared<Sym>(5))},
                {make_shared<Sym>(6), make_shared<Strength>(make_shared<Sym>(6))},
                {make_shared<Sym>(7), make_shared<Strength>(make_shared<Sym>(7))}
            };

            //static Strength absoluteWeakest = of(make_shared<Sym>(7));
            //const static Strength required = of(make_shared<Sym>(1));
    };
}

#endif //STRENGTHPLAN