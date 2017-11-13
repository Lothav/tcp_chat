#ifndef TCP_CHAT_PROTOCOL_HPP
#define TCP_CHAT_PROTOCOL_HPP

#include <cstdint>
#include <array>

namespace Common {


	struct header_str {
		uint16_t type;
		uint16_t src;
		uint16_t dest;
		uint16_t seq;
	};

	template <typename T>
	struct msg_str {
		uint16_t C;
		T text[405];
	};

	class Protocol {

	private:

		struct header_str 				header_;
		struct msg_str<char> 	        msg_str_;
		struct msg_str<uint16_t> 		msg_num_;

	public:

		Protocol(): header_({}), msg_str_({}), msg_num_({}) {}

		enum TYPE : uint16_t {
			OK 		= 1,
			ERRO 	= 2,
			OI 		= 3,
			FLW 	= 4,
			MSG 	= 5,
			CREQ 	= 6,
			CLIST 	= 7
		};

		enum ORDER : uint8_t {
			NETWORK_TO_HOST,
			HOST_TO_NETWORK
		};

		// Header setter and Getter

		void setHeader(struct header_str* header)
		{
			this->header_ = *header;
		}

		struct header_str* getHeader()
		{
			return &this->header_;
		}

		// Message setters and Getters

		void setMsg (struct msg_str<uint16_t>* msg)
		{
			this->msg_num_ = *msg;
		}

		void setMsg (struct msg_str<char>* msg)
		{
			this->msg_str_ = *msg;
		}

		void getMsg(msg_str<char>** msg)
		{
			*msg = &this->msg_str_;
		}

		void getMsg(msg_str<uint16_t>** msg)
		{
			*msg = &this->msg_num_;
		}

		// Convert functions network to host / host to network


        void convertHeaderOrder(ORDER network_order)
        {
            auto fn = network_order == ORDER::NETWORK_TO_HOST ? ntohs : htons;

            this->header_.type  = fn(this->header_.type);
            this->header_.src   = fn(this->header_.src);
            this->header_.dest  = fn(this->header_.dest);
            this->header_.seq   = fn(this->header_.seq);
        }

		void convertMsgOrder(ORDER network_order)
		{
			auto fn = network_order == ORDER::NETWORK_TO_HOST ? ntohs : htons;

			if(this->hasMsg()) {
				if (this->msgTypeNumber()) {
					msg_str<uint16_t>* msg = &this->msg_num_;
					msg->C = fn(msg->C);
					for(int i = 0; i < msg->C; i ++){
						msg->text[i] = fn(msg->text[i]);
					}
				} else {
					msg_str<char>* msg = &this->msg_str_;
					msg->C = fn(msg->C);
				}
			}
		}

		static Protocol* getProtocolFromBuffer(const char* buffer)
		{
			auto protocol = new Common::Protocol();

			std::memcpy(protocol->getHeader(), buffer, sizeof(Common::header_str));
            protocol->convertHeaderOrder(ORDER::NETWORK_TO_HOST);

            if (protocol->hasMsg()) {
				if(protocol->msgTypeNumber()) {
					msg_str <uint16_t>* msg_ = nullptr;
					protocol->getMsg(&msg_);
					std::memcpy(msg_, buffer + sizeof(Common::header_str), sizeof(Common::msg_str <uint16_t>));
				} else {
					msg_str <char>* msg_ = nullptr;
					protocol->getMsg(&msg_);
					std::memcpy(msg_, buffer + sizeof(Common::header_str), sizeof(Common::msg_str <char>));
				}
			}
			protocol->convertMsgOrder(ORDER::NETWORK_TO_HOST);

			return protocol;
		}

		bool hasMsg()
		{
			return this->header_.type == Common::Protocol::TYPE::MSG ||
					this->header_.type == Common::Protocol::TYPE::CLIST ||
					ntohs(this->header_.type) == Common::Protocol::TYPE::MSG ||
					ntohs(this->header_.type) == Common::Protocol::TYPE::CLIST;
		}

		bool msgTypeNumber()
		{
			return this->header_.type == Common::Protocol::TYPE::CLIST ||
					ntohs(this->header_.type) == Common::Protocol::TYPE::CLIST;;
		}

	};

}

#endif //TCP_CHAT_PROTOCOL_HPP
