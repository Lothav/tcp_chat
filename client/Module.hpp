//
// Created by tracksale on 10/31/17.
//

#ifndef TCP_CHAT_MODULE_HPP
#define TCP_CHAT_MODULE_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include "../common/Socket.hpp"

namespace Server {

    class Module : Common::Socket
    {

    public:
        explicit Module(std::string port)
        {
            struct sockaddr_in dst = {};
            dst.sin_family  = AF_INET;
            dst.sin_port    = htons(static_cast<uint16_t>(std::stoi(port.c_str())));
            inet_aton(LOCAL_HOST, &dst.sin_addr);

            auto *sa_dst = (struct sockaddr *)&dst;
            while(connect(this->getSocket(), sa_dst, sizeof(dst))) {}
        }

        void run() {

        }

    };

}

#endif //TCP_CHAT_MODULE_HPP
