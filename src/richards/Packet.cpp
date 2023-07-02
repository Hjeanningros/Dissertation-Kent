#include <vector>
#include "RBObject.cpp"
#include <memory>
#include <string>

namespace richards {

    class Packet : public RBObject {
        private:
            std::shared_ptr<Packet> _link;
            int _identity;
            int _kind;
            int _datum;
            std::vector<int> _data;


        public:
            static const int DATA_SIZE = 4;

            Packet(Packet link, int identity, int kind) {
                _link = std::make_shared<Packet>(link);
                _identity = identity;
                _kind = kind;
                _datum = 0;
                _data = std::vector<int>(DATA_SIZE);
            }

            std::vector<int> getData() { 
                return _data; 
            }

            int getDatum() { 
                return _datum; 
            }

            void setDatum(int someData) { 
                _datum = someData; 
            }


            int getIdentity() { 
                return _identity; 
            }
            
            void setIdentity(int anIdentity) { 
                _identity = anIdentity; 
            }

            int getKind() { 
                return _kind; 
            }
            
            std::shared_ptr<Packet> getLink() { 
                return _link; 
            }
            
            void setLink(Packet aLink) { 
                _link = std::make_shared<Packet>(aLink); 
            }

            std::string toString() {
                return "Packet id: " + _identity + " kind: " + _kind;
            }
    }

}