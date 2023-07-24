#include <vector>
#include "JsonValue.h"
#include <memory>
#include "../som/Error.cpp"

namespace json {
    class JsonArray : public JsonValue {
        private:
            vector<shared_ptr<JsonValue>> _values;
        
        public:

            JsonArray() {
                _values = vector<shared_ptr<JsonValue>>();
            }

            shared_ptr<JsonArray> add(shared_ptr<JsonValue> value) {
                if (value == nullptr) {
                    throw Error("value is null");
                }
                _values.push_back(value);
                return shared_ptr<JsonArray>(this);
            }

            int size() {
                return _values.size();
            }

            shared_ptr<JsonValue> get(int index) {
                return _values[index];
            }

            bool isArray() override {
                return true;
            }

            shared_ptr<JsonArray> asArray() override {
                return shared_ptr<JsonArray>(this);
            }
    };
}