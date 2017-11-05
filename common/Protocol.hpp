#ifndef TCP_CHAT_PROTOCOL_HPP
#define TCP_CHAT_PROTOCOL_HPP

#include <cstdint>
#include <array>
#include <netinet/in.h>

namespace Common {

    struct header_str {
        uint16_t type;
        uint16_t src;
        uint16_t dest;
        uint16_t seq;
    };

    struct msg_str {
        uint16_t C;
        char msg[401];
    };

    class Protocol {

    private:

        struct header_str _header;
        struct msg_str _msg;

    public:
        Protocol(): _header({}), _msg({}) {}

        void headerToHostOrder()
        {
            this->_header.type  = ntohs(this->_header.type);
            this->_header.src   = ntohs(this->_header.src);
            this->_header.dest  = ntohs(this->_header.dest);
            this->_header.seq   = ntohs(this->_header.seq);
        }

        void msgToHostOrder()
        {
            this->_msg.C = ntohs(this->_msg.C);
        }

        bool hasMsg()
        {
            return this->_header.type == 5;
        }

        struct header_str* getHeader()
        {
            return &this->_header;
        }

        struct msg_str* getMsg()
        {
            return &this->_msg;
        }
    };

}

#endif //TCP_CHAT_PROTOCOL_HPP
