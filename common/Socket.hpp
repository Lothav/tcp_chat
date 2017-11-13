//
// Created by tracksale on 10/31/17.
//

#ifndef TCP_CHATsocket__HPP
#define TCP_CHATsocket__HPP


#include "Protocol.hpp"

#define LOCAL_HOST "127.0.0.1"
#define TC_INVALID_SOCKET 0

namespace Common {

    class Socket {

        int socket_;

    public:

		enum EVENT_TYPE {
			KEYBOARD 	= 0x1 << 0,
			RECEIVE 	= 0x1 << 1,
			ACCEPT 		= 0x1 << 2
		};

    protected:

		std::vector<uint16_t> clients_sockets_ = {TC_INVALID_SOCKET};

		Socket()
        {
            socket_ = socket(AF_INET, SOCK_STREAM, 0);
            if (socket_ < 0) {
                throw std::runtime_error("ERROR opening socket");
            }

        }

        int getSocket()
        {
            return socket_;
        }

		void tcpSelect(int m_socket_,  const std::function<void(int, int)>& handler = nullptr)
		{
			struct timeval tv = {};
			tv.tv_sec = 5;
			tv.tv_usec = 0;

			while (true) {

				fd_set master = {};
				fd_set rfds = {};

				FD_ZERO(&master);
				FD_ZERO(&rfds);

				int nfds = m_socket_;
				FD_SET(m_socket_, &master);
				FD_SET(STDIN_FILENO, &master);
				for (auto c_socket : clients_sockets_) {
					nfds = c_socket > nfds ? c_socket : nfds;
					FD_SET(c_socket, &master);
				}

				rfds = master;
				int retval = select(nfds+1, &rfds, nullptr, nullptr, &tv);
				if (retval > 0) {
					if (FD_ISSET(STDIN_FILENO, &rfds)) {
						handler(EVENT_TYPE::KEYBOARD, m_socket_);
					}
					if (FD_ISSET(m_socket_, &rfds)) {
						handler(EVENT_TYPE::ACCEPT | EVENT_TYPE::RECEIVE, m_socket_);
					}
					if (clients_sockets_.size() > 1) {
						for (auto c_socket : clients_sockets_) {
							if (FD_ISSET(c_socket, &rfds)) {
								handler(EVENT_TYPE::RECEIVE, c_socket);
							}
						}
					}
				}
			}
		}

        void tcpSend (int socket_, const std::shared_ptr<Common::Protocol>& protocol_)
        {
			bool hasMsg = protocol_->hasMsg();
			protocol_->convertHeaderOrder(Protocol::ORDER::HOST_TO_NETWORK);

			if (hasMsg) {

                protocol_->convertMsgOrder(Protocol::ORDER::HOST_TO_NETWORK);
                if (protocol_->msgTypeNumber()) {

                    size_t send_size = sizeof(header_str) + sizeof(msg_str<std::uint16_t>);
					std::unique_ptr<char> buffer((char *) malloc(send_size));
                	memcpy(buffer.get(), protocol_->getHeader(), sizeof(header_str));

                	Common::msg_str<uint16_t>* str = nullptr;
                	protocol_->getMsg(&str);
                	memcpy(buffer.get()+sizeof(header_str), str, sizeof(msg_str<std::uint16_t>));

                    send(socket_, buffer.get(), send_size, 0);
                } else {

                    size_t send_size = sizeof(header_str) + sizeof(msg_str<char>);
                    std::unique_ptr<char> buffer ((char *)malloc(send_size));
                    memcpy(buffer.get(), protocol_->getHeader(), sizeof(header_str));
                    
                    Common::msg_str<char>* str = nullptr;
                    protocol_->getMsg(&str);
                    memcpy(buffer.get()+sizeof(header_str), str, sizeof(msg_str<char>));

                    send(socket_, buffer.get(), send_size, 0);
                }

			} else {
				size_t send_size = sizeof(header_str);
				std::unique_ptr<char> buffer((char *) malloc(send_size));
				memcpy(buffer.get(), protocol_->getHeader(), send_size);

				send(socket_, buffer.get(), send_size, 0);
			}
        }

		Common::Protocol* receive (int socket)
        {
            //@TODO FIX SIZEOF MSG
            size_t buffer_size = sizeof(Common::header_str) + sizeof(Common::msg_str<uint16_t>);
            std::unique_ptr<char> buffer ((char *)malloc(buffer_size));
            ssize_t recv_size = recv(socket, buffer.get(), buffer_size, 0);

            if (recv_size > 0) {
                return Common::Protocol::getProtocolFromBuffer(buffer.get());
            } else {
				close(socket);
                std::cout << "Conexão com socket " << socket << " encerrada." << std::endl;
            }
        }
    };
}

#endif //TCP_CHATsocket__HPP
