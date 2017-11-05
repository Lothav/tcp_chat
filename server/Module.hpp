//
// Created by tracksale on 10/31/17.
//

#ifndef TCP_CHAT_MODULE_HPP
#define TCP_CHAT_MODULE_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <cstring>
#include "../common/Socket.hpp"
#include "../common/Protocol.hpp"

#define TC_MAX_REQUESTS 5

namespace Server {

    class Module : Common::Socket
    {

    public:
        explicit Module(char* port)
        {
            int size = 1;
            int m_socket = this->getSocket();
            if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &size, sizeof(int)) < 0)
                printf("setsockopt(SO_REUSEADDR) failed");

            struct sockaddr_in serv_addr = {};
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port   = htons(static_cast<uint16_t>(std::atoi(port)));
            inet_aton(LOCAL_HOST, &serv_addr.sin_addr);

            while (bind(m_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0);

            listen(m_socket, TC_MAX_REQUESTS);

            this->tcpSelect(m_socket, this->clients_sockets_, [this](int event_mask, int socket_) {
                this->handleEvent(event_mask, socket_);
            });
        }

    private:

        std::vector<int> clients_sockets_ = {};

        void handleEvent(int event_mask, int socket_)
        {
            if ((event_mask & Common::Socket::EVENT_TYPE::ACCEPT) == Common::Socket::EVENT_TYPE::ACCEPT) {
                struct sockaddr_in 	cli_addr = {};
                socklen_t clilen = sizeof(cli_addr);
                socket_ = accept(this->getSocket(), (struct sockaddr *)&cli_addr, &clilen);
            }

            if ((event_mask & Common::Socket::EVENT_TYPE::RECEIVE) == Common::Socket::EVENT_TYPE::RECEIVE) {
                Common::Protocol* protocol_ = this->receive(socket_);
                Common::header_str* header_ = protocol_->getHeader();

                std::cout<< "type: " << header_->type << std::endl;

                switch (header_->type)
                {
                    case Common::Protocol::TYPE::OI:
                        this->clients_sockets_.push_back(socket_);

                        header_->type = Common::Protocol::TYPE::OK;
                        header_->dest = static_cast<uint16_t>(this->clients_sockets_.size());
                        protocol_->setHeader(header_);
                        
                        this->tcpSend(socket_, protocol_);

                        break;
                    default:
                        break;
                }
            }
        }


    };
}

#endif //TCP_CHAT_MODULE_HPP
