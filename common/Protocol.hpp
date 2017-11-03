#ifndef TCP_CHAT_PROTOCOL_HPP
#define TCP_CHAT_PROTOCOL_HPP

#include <cstdint>
#include <array>
#include <netinet/in.h>

namespace Common {

    class Protocol {

    public:

        uint16_t type;
        uint16_t src;
        uint16_t dest;
        uint16_t seq;

        Protocol() : type(), src(), dest(), seq() {

        }

        void toHostOrder()
        {
            this->type  = ntohs(this->type);
            this->src   = ntohs(this->src);
            this->dest  = ntohs(this->dest);
            this->seq   = ntohs(this->seq);
        }
    };

}

#endif //TCP_CHAT_PROTOCOL_HPP
