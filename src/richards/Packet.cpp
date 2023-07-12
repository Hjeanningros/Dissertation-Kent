#include "Packet.h"

#include <utility>

namespace richards {
    Packet::Packet(std::shared_ptr<Packet> link, int identity, int kind) {
        _link = std::move(link);
        _identity = identity;
        _kind = kind;
        _datum = 0;
        _data = std::vector<int>(DATA_SIZE);
    }

    std::vector<int> Packet::getData() { 
        return _data; 
    }

    int Packet::getDatum() const {
        return _datum; 
    }

    void Packet::setDatum(int someData) { 
        _datum = someData; 
    }


    int Packet::getIdentity() const {
        return _identity; 
    }
            
    void Packet::setIdentity(int anIdentity) { 
        _identity = anIdentity; 
    }

    int Packet::getKind() const {
        return _kind; 
    }
            
    std::shared_ptr<Packet> Packet::getLink() { 
        return _link; 
    }
            
    void Packet::setLink(std::shared_ptr<Packet> aLink) { 
        _link = std::move(aLink);
    }

    std::string Packet::toString() const {
        return "Packet id: " + std::to_string(_identity) + " kind: " + std::to_string(_kind);
    }
}
