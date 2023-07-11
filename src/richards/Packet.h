#ifndef PACKET
#define PACKET

#include <vector>
#include <string>
#include <memory>
#include "RBObject.h"

namespace richards {

    class Packet : public RBObject {
        private:
            std::shared_ptr<Packet> _link;
            int _identity;
            int _kind;
            int _datum;
            std::vector<int> _data;

        public:

            Packet(std::shared_ptr<Packet> link, int identity, int kind);

            std::vector<int> getData();
            int getDatum() const;
            void setDatum(int someData);
            int getIdentity() const;
            void setIdentity(int anIdentity);
            int getKind() const;
            std::shared_ptr<Packet> getLink();
            void setLink(std::shared_ptr<Packet> aLink);
            std::string toString() const;

            static const int DATA_SIZE = 4;
    };
}

#endif //PACKET