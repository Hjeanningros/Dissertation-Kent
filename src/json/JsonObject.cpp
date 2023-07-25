#include <vector>
#include "JsonValue.h"
#include <string>
#include <memory>
#include <map>
#include "../som/Error.cpp"
using namespace std;

namespace json {
    class JsonObject : public JsonValue {
        private:
            vector<string> _names;
            vector<shared_ptr<JsonValue>> _values;
            map<string, int> _table;

            int indexOf(string name) {
                int index = _table.find(name)->second;
                if (index != -1 && name == _names[index]) {
                    return index;
                }
                throw new Error ("NotImplemented"); // Not needed for benchmark
            }

        public:
            JsonObject() = default;

            shared_ptr<JsonObject> add(string name, const shared_ptr<JsonValue> value) {
                if (name.empty()) {
                    throw Error ("name is null");
                }
                if (!value) {
                    throw Error ("value is null");
                }
                _table[name] = _names.size();
                _names.push_back(name);
                _values.push_back(value);
                return shared_ptr<JsonObject>(this);
            }

            shared_ptr<JsonValue> get(string name) {
                if (name.empty()) {
                    throw Error ("name is null");
                }
                int index = indexOf(name);
                return index == -1 ? nullptr : _values[index];
            }

            int size() const {
                return static_cast<int>(_names.size());
            }

            bool isEmpty() const {
                return _names.empty();
            }

            bool isObject() override {
                return true;
            }

            shared_ptr<JsonValue> get(string name) const {
                auto it = _table.find(name);
                if (it != _table.end()) {
                    int index = it->second;
                    if (index >= 0 && index < static_cast<int>(_values.size())) {
                        return _values[index];
                    }
                }
                return nullptr;
            }



            shared_ptr<JsonObject> asObject() override {
                return shared_ptr<JsonObject>(this);
            }
    };
}