//
// Created by tracksale on 10/31/17.
//

#ifndef TCP_CHAT_MODULE_HPP
#define TCP_CHAT_MODULE_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include "../common/Socket.hpp"

namespace Client {

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

            fd_set rfds = {};
            struct timeval tv = {};
            char buf[35];

            while(1) {
                FD_ZERO(&rfds);                     // Clear an fd_set
                FD_SET(STDIN_FILENO, &rfds);        // Add a descriptor to an fd_set
                FD_SET(this->getSocket(), &rfds);
                tv.tv_sec = 5;
                tv.tv_usec = 0;
                int  retval = select(3, &rfds, nullptr, nullptr, &tv);
                if (FD_ISSET(STDIN_FILENO, &rfds)) {
                    if (fgets(buf, 35, stdin)) {
                        printf("%d %s\n",__LINE__ ,buf);
                    }
                    printf("\nEnter command: ");
                }
            }
        }
    };

}

#endif //TCP_CHAT_MODULE_HPP
