#ifndef TCP_CHAT_PROTOCOL_HPP
#define TCP_CHAT_PROTOCOL_HPP

#include <cstdint>
#include <array>

namespace Common {

    class Protocol {

    public:

        uint16_t type;
        uint16_t src;
        uint16_t dest;
        uint16_t seq;

        Protocol() : type(), src(), dest(), seq() {

        }

        void ahehae() {

        }
        void aahehae() {

        }
    };

}

#endif //TCP_CHAT_PROTOCOL_HPP
