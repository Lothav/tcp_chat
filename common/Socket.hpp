//
// Created by tracksale on 10/31/17.
//

#ifndef TCP_CHATsocket__HPP
#define TCP_CHATsocket__HPP

#include <sys/socket.h>
#include <stdexcept>
#include "Protocol.hpp"

#define LOCAL_HOST "127.0.0.1"

namespace Common {

    class Socket {

        int socket_;

    public:


    protected:

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

        void tcpSend(int socket_, Common::Protocol* protocol_)
        {
            size_t send_size = protocol_->hasMsg() ? sizeof(header_str) + sizeof(msg_str) : sizeof(header_str);
            send(socket_, protocol_, send_size, 0);
        }

        void receive (int socket)
        {
            size_t buffer_size = sizeof(Common::header_str) + sizeof(Common::msg_str);
            std::unique_ptr<char> buffer ((char *)malloc(buffer_size));
            ssize_t recv_size = recv(socket, buffer.get(), buffer_size, 0);

            if (recv_size > 0) {
                std::unique_ptr<Common::Protocol> protocol( new Common::Protocol() );

                std::memcpy(protocol->getHeader(), buffer.get(), sizeof(Common::header_str));
                protocol->headerToHostOrder();

                if (protocol->hasMsg()) {
                    std::memcpy(protocol->getMsg(), buffer.get()+sizeof(Common::header_str), sizeof(Common::msg_str));
                    protocol->msgToHostOrder();
                }

                std::cout << protocol->getHeader()->type << " " << protocol->getHeader()->src << " " << protocol->getHeader()->dest << " " << protocol->getHeader()->seq << " "  << std::endl;
            } else {
                throw "asd";
            }
        }

    };
}

#endif //TCP_CHATsocket__HPP
