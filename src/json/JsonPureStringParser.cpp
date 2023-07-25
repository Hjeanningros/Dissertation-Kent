#include <string>
#include "../som/Error.cpp"
#include <memory>
#include "JsonArray.cpp"
#include "JsonObject.cpp"
#include "ParseException.cpp"
#include "JsonLiteral.h"
#include "JsonNumber.cpp"
#include "JsonString.cpp"

#include <iostream>

using namespace std;

namespace json {
    class JsonPureStringParser {
        private:
            string _input;
            int _index;
            int _line;
            int _column;
            string _current;
            string _captureBuffer;
            int _captureStart;

            shared_ptr<JsonValue> readValue() {
                cout << "current = " << _current << endl;
                if (_current == "n") 
                    return readNull();
                else if (_current == "t") 
                    return readTrue();
                else if (_current == "f") 
                    return readFalse();
                else if (_current == "\"") 
                    return readString();
                else if (_current == "[") 
                    return readArray();
                else if (_current == "{") {
                    return readObject();
                }
                else if (_current == "-" || _current == "0" || _current == "1" || _current == "2" || _current == "3" || _current == "4"
                    || _current == "5" || _current == "6" || _current == "7" || _current == "8" || _current == "9")
                    readNumber();
                else 
                    throw expected("value");
            }

            shared_ptr<JsonObject> readObject() {
                read();
                shared_ptr<JsonObject> object = make_shared<JsonObject>();
                skipWhiteSpace();
                if (readChar("}")) {
                    return object;
                }
                do {
                    skipWhiteSpace();
                    string name = readName();
                    skipWhiteSpace();
                    if (!readChar(":")) {
                        throw expected("':'");
                    }
                    skipWhiteSpace();
                    object->add(name, readValue());
                    skipWhiteSpace();
                } 
                while (readChar(","));
                if (!readChar("}")) {
                    throw expected("',' or '}'");
                }
                return object;
            }

            string readName() {
                if (_current != "\"") {
                    throw expected("name");
                }
                return readStringInternal();
            }

            shared_ptr<JsonArray> readArray() {
                read();
                shared_ptr<JsonArray> array = make_shared<JsonArray>();
                skipWhiteSpace();
                if (readChar("]")) {
                    return array;
                }
                do {
                    skipWhiteSpace();
                    array->add(readValue());
                    skipWhiteSpace();
                }  while (readChar(","));

                if (!readChar("]")) {
                        throw expected("',' or ']'");
                }
                return array;
            }

            shared_ptr<JsonValue> readNull() {
                read();
                readRequiredChar("u");
                readRequiredChar("l");
                readRequiredChar("l");
                return JsonLiteral::NNULL;
            }

            shared_ptr<JsonValue> readTrue() {
                read();
                readRequiredChar("r");
                readRequiredChar("u");
                readRequiredChar("e");
                return JsonLiteral::TRUE;
            }

            shared_ptr<JsonValue> readFalse() {
                read();
                readRequiredChar("a");
                readRequiredChar("l");
                readRequiredChar("s");
                readRequiredChar("e");
                return JsonLiteral::FALSE;
            }

            void readRequiredChar(string ch) {
                if (!readChar(ch)) {
                    throw expected("'" + ch + "'");
                }
            }

            shared_ptr<JsonValue> readString() {
                return make_shared<JsonString>(readStringInternal());
            }

            string readStringInternal() {
                read();
                startCapture();
                while (_current != "\"") {
                    if (_current == "\\") {
                        pauseCapture();
                        readEscape();
                        startCapture();
                    } else {
                        read();
                    }
                }
                string string = endCapture();
                read();
                return string;
            }

            void readEscape() {
                read();
                if (_current == "\"" || _current == "/" || _current == "\\") 
                    _captureBuffer += _current;
                else if (_current == "b")
                    _captureBuffer += "\b";
                else if (_current == "f")
                    _captureBuffer += "\f";
                else if (_current == "n")
                    _captureBuffer += "\n";
                else if (_current == "r")
                    _captureBuffer += "\r";
                else if (_current == "t")
                    _captureBuffer += "\t";
                else 
                    throw expected("valid escape sequence");
                read();
            }

            shared_ptr<JsonValue> readNumber() {
                startCapture();
                readChar("-");
                string firstDigit = _current;
                if (!readDigit()) {
                    throw expected("digit");
                }
                if (firstDigit != "0") {
                    while (readDigit()) { }
                }
                readFraction();
                readExponent();
                return make_shared<JsonNumber>(endCapture());
            }

            bool readFraction() {
                if (!readChar(".")) {
                    return false;
                }
                if (!readDigit()) {
                    throw expected("digit");
                }
                while (readDigit()) { }
                return true;
            }

            bool readExponent() {
                if (!readChar("e") && !readChar("E")) {
                    return false;
                }
                if (!readChar("+")) {
                    readChar("-");
                }
                if (!readDigit()) {
                    throw expected("digit");
                }

                while (readDigit()) { }
                return true;
            }

            bool readChar(string ch) {
                if (_current != ch) {
                    return false;
                }
                read();
                return true;
            }

            bool readDigit() {
                if (!isDigit()) {
                    return false;
                }
                read();
                return true;
            }

            void skipWhiteSpace() {
                while (isWhiteSpace()) {
                    read();
                }
            }

            void read() {
                if ("\n" == _current) {
                    _line++;
                    _column = 0;
                }
                _index++;
                if (_index < _input.length()) {
                    _current = _input.substr(_index, 1);
                } else {
                    _current = "";
                }
            }

            void startCapture() {
                _captureStart = _index;
            }

            void pauseCapture() {
                int _end = _current == "" ? _index : _index - 1;
                _captureBuffer += _input.substr(_captureStart, _end - _captureStart + 1);
                _captureStart = -1;
            }

            string endCapture() {
                int _end = _current == "" ? _index : _index - 1;
                string captured;
                if ("" == _captureBuffer) {
                    captured = _input.substr(_captureStart, _end - _captureStart + 1);
                } else {
                    _captureBuffer += _input.substr(_captureStart, _end - _captureStart + 1);
                    captured = _captureBuffer;
                    _captureBuffer = "";
                }
                _captureStart = -1;
                return captured;
            }

            ParseException expected(string expected) {
                if (isEndOfText()) {
                    return error("Unexpected end of input");
                }

                return error("Expected " + expected);
            }

            ParseException error(string message) {
                return ParseException(message, _index, _line, _column - 1);
            }

            bool isWhiteSpace() {
                return " " == _current || "\t" == _current || "\n" == _current || "\r" == _current;
            }


            bool isDigit() {
                return "0" == _current ||
                    "1" == _current ||
                    "2" == _current ||
                    "3" == _current ||
                    "4" == _current ||
                    "5" == _current ||
                    "6" == _current ||
                    "7" == _current ||
                    "8" == _current ||
                    "9" == _current;
            }

            bool isEndOfText() {
                return _current == "";
            }

        public: 
            JsonPureStringParser(string string) {
                _input = string;
                _index = -1;
                _line = 1;
                _captureStart = -1;
                _column = 0;
                _current = "";
                _captureBuffer = "";
            }

            

            shared_ptr<JsonValue> parse() {
                cout << "HAHAHHAHA" << endl;
                read();
                cout << "HAHAHHAHA" << endl;
                skipWhiteSpace();
                cout << "HAHAHHAHA" << endl;
                shared_ptr<JsonValue> result = readValue();
                cout << "HAHAHHAHA" << endl;
                skipWhiteSpace();
                if (!isEndOfText()) {
                    throw Error("Unexpected character");
                }
                return result;
            }



    };
}
