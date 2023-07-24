#include "JsonLiteral.h"


namespace json {
    
    extern shared_ptr<JsonLiteral> JsonLiteral::NNULL = make_shared<JsonLiteral>("null");
    extern shared_ptr<JsonLiteral> JsonLiteral::TRUE = make_shared<JsonLiteral>("true");
    extern shared_ptr<JsonLiteral> JsonLiteral::FALSE = make_shared<JsonLiteral>("false");

    JsonLiteral::JsonLiteral(string value) {
        _value = value;
        _isNull = value == "null";
        _isTrue = value == "true";
        _isFalse = value == "false";
    }

    // Method to get the string representation of the literal
    string JsonLiteral::toString() {
        return _value;
    }

    // Check if the literal is null
    bool JsonLiteral::isNullValue() {
        return _isNull;
    }

    // Check if the literal is true
    bool JsonLiteral::isTrueValue() {
        return _isTrue;
    }

    // Check if the literal is false
    bool JsonLiteral::isFalseValue() {
        return _isFalse;
    }

    // Check if the literal is a boolean value (true or false)
    bool JsonLiteral::isBooleanValue() {
        return _isTrue || _isFalse;
    }

}