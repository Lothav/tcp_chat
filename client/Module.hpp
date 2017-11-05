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

namespace Client {

    class Module : Common::Socket
    {

    public:
        explicit Module(std::string port)
        {
            int m_socket_ = this->getSocket();

            struct sockaddr_in dst = {};
            dst.sin_family  = AF_INET;
            dst.sin_port    = htons(static_cast<uint16_t>(std::stoi(port.c_str())));
            inet_aton(LOCAL_HOST, &dst.sin_addr);

            auto *sa_dst = (struct sockaddr *)&dst;
            while(connect(m_socket_, sa_dst, sizeof(dst)));
            this->sayHi();

            this->tcpSelect(m_socket_, {}, [this](int event_mask, int socket_) {
                this->handleEvent(event_mask, socket_);
            });
        }


    private:

        void handleEvent(int event_mask, int socket_)
        {
            if ((event_mask & Common::Socket::EVENT_TYPE::ACCEPT) == Common::Socket::EVENT_TYPE::ACCEPT) {

            }
        }

        void sayHi ()
        {
            std::unique_ptr<Common::header_str> header_hi (new Common::header_str);
            header_hi->type = htons(3);
            header_hi->src  = htons(0);
            header_hi->dest = htons(0);
            header_hi->seq  = htons(0);

            send(this->getSocket(), header_hi.get(), sizeof(Common::header_str), 0);
        }
    };

}

#endif //TCP_CHAT_MODULE_HPP
