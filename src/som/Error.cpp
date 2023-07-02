
#ifndef ERROR
#define ERROR


#include <string>


class Error : virtual public std::exception {
    private:
        std::string _what;

    public:
        Error(std::string const &what) {
            _what = what;
        }
        
        Error(const char *what) {
            _what = std::string(what);
        }

        const char *what() const throw() {
            return (_what.c_str());
        }
};

#endif //ERROR