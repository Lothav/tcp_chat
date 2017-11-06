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

		enum TYPE : uint16_t {
			OK 		= 1,
			ERRO 	= 2,
			OI 		= 3,
			FLW 	= 4,
			MSG 	= 5,
			CREQ 	= 6,
			CLIST 	= 7
		};

		void setHeader(struct header_str* header)
		{
			this->_header = *header;
		}

		void setMsg (struct msg_str* msg)
		{
			this->_msg = *msg;
		}

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

		void headerToNetworkOrder()
		{
			this->_header.type  = htons(this->_header.type);
			this->_header.src   = htons(this->_header.src);
			this->_header.dest  = htons(this->_header.dest);
			this->_header.seq   = htons(this->_header.seq);
		}

		void msgToNetworkOrder()
		{
			this->_msg.C = htons(this->_msg.C);
		}

		static Protocol* getProtocolFromBuffer(const char* buffer)
		{
			auto protocol = new Common::Protocol();

			std::memcpy(protocol->getHeader(), buffer, sizeof(Common::header_str));
			protocol->headerToHostOrder();

			if (protocol->hasMsg()) {
				std::memcpy(protocol->getMsg(), buffer+sizeof(Common::header_str), sizeof(Common::msg_str));
				protocol->msgToHostOrder();
			}

			return protocol;
		}

        bool hasMsg()
        {
            return this->_header.type == Common::Protocol::TYPE::MSG;
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
