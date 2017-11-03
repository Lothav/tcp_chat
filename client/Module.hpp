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
            int socket_server = this->getSocket();

            struct sockaddr_in dst = {};
            dst.sin_family  = AF_INET;
            dst.sin_port    = htons(static_cast<uint16_t>(std::stoi(port.c_str())));
            inet_aton(LOCAL_HOST, &dst.sin_addr);

            auto *sa_dst = (struct sockaddr *)&dst;
            while(connect(socket_server, sa_dst, sizeof(dst)));

            struct timeval tv = {};
            char buf[35];

            fd_set master;
            fd_set rfds;

            FD_ZERO(&master);
            FD_ZERO(&rfds);

            FD_SET(STDIN_FILENO, &master);
            FD_SET(socket_server, &master); // s is a socket descriptor

            tv.tv_sec = 5;
            tv.tv_usec = 0;

            while(true) {
                rfds = master;

                int retval = select(socket_server+1, &rfds, nullptr, nullptr, nullptr);
                std::cout << retval << std::endl;
                if (retval > 0) {
                    if (FD_ISSET(STDIN_FILENO, &rfds)) {
                        if (fgets(buf, 35, stdin)) {
                            Common::Protocol *protocol = new Common::Protocol(1,2,5,3);
                            send(socket_server, buf, strlen(buf), 0);
                            std::cout << buf << std::endl;
                        }
                    }
                    if (FD_ISSET(socket_server, &rfds)) {
                        int ret = recv(socket_server, buf, 4, 0);
                        std::cout << buf << std::endl;
                    }
                }
            }
        }
    };

}

#endif //TCP_CHAT_MODULE_HPP
