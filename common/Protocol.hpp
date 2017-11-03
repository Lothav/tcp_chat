#ifndef TCP_CHAT_PROTOCOL_HPP
#define TCP_CHAT_PROTOCOL_HPP

#include <cstdint>
#include <array>

namespace Common {

    class Protocol {

    private:

        uint16_t type;
        uint16_t id;
        uint16_t dest;
        uint16_t seq;

    public:

        Protocol(uint16_t _type, uint16_t id, uint16_t dest, uint16_t seq): type(_type), id(id), dest(dest), seq(seq){}

        void ahehae() {

        }
        void aahehae() {

        }
    };

}

#endif //TCP_CHAT_PROTOCOL_HPP
