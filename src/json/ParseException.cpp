#include <string>

using namespace std;

namespace json {
    class ParseException : virtual public exception {
        private:
            int _offset;
            int _line;
            int _column;
            string _message;

        public:
            ParseException(string message, int offset, int line, int column) {
                _offset = offset;
                _line   = line;
                _column = column;
                _message = message;
            }
            
            int getOffset() {
                return _offset;
            }

            int getLine() {
                return _line;
            }

            int getColumn() {
                return _column;
            }

            const char *what() const throw() {
                string full_message = _message + " at " + to_string(_line) + ":" + to_string(_column);
                return (full_message.c_str());
            }
    };
}