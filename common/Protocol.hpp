#ifndef TCP_CHAT_PROTOCOL_HPP
#define TCP_CHAT_PROTOCOL_HPP

#include <cstdint>

namespace Common {

    enum MSG_TYPE {
        OK      = 1,
        ERRO    = 2,
        OI      = 3,
        FLW     = 4,
        MSG     = 5,
        CREQ    = 6,
        CLIST   = 7
    };

    class Protocol {

    private:

        uint16_t type;
        uint16_t id;
        uint16_t dest;
        uint16_t seq;

    public:
        Protocol(uint16_t _type, uint16_t id, uint16_t dest, uint16_t seq): type(_type), id(id), dest(dest), seq(seq){}
    };

}

#endif //TCP_CHAT_PROTOCOL_HPP
