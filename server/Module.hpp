//
// Created by tracksale on 10/31/17.
//

#ifndef TCP_CHAT_MODULE_HPP
#define TCP_CHAT_MODULE_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <cstring>
#include <algorithm>
#include <sstream>
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

            this->tcpSelect(m_socket, [this](int event_mask, int socket_) {
                this->handleEvent(event_mask, socket_);
            });
        }

    private:

        void handleEvent(int event_mask, int socket_)
        {
            if ((event_mask & Common::Socket::EVENT_TYPE::ACCEPT) == Common::Socket::EVENT_TYPE::ACCEPT) {
                struct sockaddr_in 	cli_addr = {};
                socklen_t clilen = sizeof(cli_addr);
                socket_ = accept(this->getSocket(), (struct sockaddr *)&cli_addr, &clilen);
            }

            if ((event_mask & Common::Socket::EVENT_TYPE::RECEIVE) == Common::Socket::EVENT_TYPE::RECEIVE) {
                std::shared_ptr<Common::Protocol> protocol_ (this->receive(socket_));
                Common::header_str* header_ = protocol_->getHeader();

                switch (header_->type)
                {
                    case Common::Protocol::TYPE::OI:

                        header_->type = Common::Protocol::TYPE::OK;
                        header_->dest = static_cast<uint16_t>(this->clients_sockets_.size());
                        protocol_->setHeader(header_);

                        this->clients_sockets_.push_back(socket_);

                        this->tcpSend(socket_, std::move(protocol_));
                        break;

                    case Common::Protocol::TYPE::FLW:

                        clients_sockets_[header_->src] = TC_INVALID_SOCKET;
                        header_->type = Common::Protocol::TYPE::OK;
                        protocol_->setHeader(header_);
                        this->tcpSend(socket_, std::move(protocol_));
                        close(socket_);
                        break;

                    case Common::Protocol::TYPE::MSG:

                        if (header_->dest == 0) {
                            // Broadcast
                            for (auto clients_sockets : clients_sockets_) {
                                if (clients_sockets != TC_INVALID_SOCKET && clients_sockets != clients_sockets_[header_->src]) {
                                    this->tcpSend(clients_sockets, std::move(protocol_));
                                }
                            }
                            header_->type = Common::Protocol::TYPE::OK;
                            protocol_->setHeader(header_);
                            this->tcpSend(socket_, std::move(protocol_));
                        } else {
                            if(clients_sockets_[header_->dest] == 0) {
                                // invalid destination
                                header_->type = Common::Protocol::TYPE::ERRO;
                                protocol_->setHeader(header_);
                                this->tcpSend(socket_, std::move(protocol_));
                            } else {
                                // send to header_->dest
                                this->tcpSend(clients_sockets_[header_->dest], std::move(protocol_));
                                // confirm to socket_ OK
                                header_->type = Common::Protocol::TYPE::OK;
                                protocol_->setHeader(header_);
                                this->tcpSend(socket_, std::move(protocol_));
                            }
                        }
                        break;

                    case Common::Protocol::TYPE::CREQ:
                    {
                        header_->type = Common::Protocol::TYPE::CLIST;
                        header_->dest = header_->src;
                        header_->src = 0;

                        Common::msg_str<uint16_t>* msg_;
                        protocol_->getMsg(&msg_);

                        std::vector<uint16_t> ids = {};
                        uint16_t i = 0;
                        for(i = 0; i < clients_sockets_.size(); i++){
                            if(clients_sockets_[i] != TC_INVALID_SOCKET){
                                ids.push_back(i);
                            }
                        }
                        msg_->C = static_cast<uint16_t>( ids.size() );
                        memcpy(&(msg_->text), ids.data(), sizeof(uint16_t) * ids.size());
                        this->tcpSend(socket_, std::move(protocol_));

                        break;
                    }
                    default:

                        std::cout << "Invalid Type";
                        header_->type = Common::Protocol::TYPE::ERRO;
                        protocol_->setHeader(header_);
                        this->tcpSend(socket_, std::move(protocol_));

                        break;
                }
            }
        }


    };
}

#endif //TCP_CHAT_MODULE_HPP
