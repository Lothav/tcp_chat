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
            /*

            char buf[35];

            fd_set master = {};
            fd_set rfds = {};

            FD_ZERO(&master);
            FD_ZERO(&rfds);

            FD_SET(STDIN_FILENO, &master);
            FD_SET(socket_server, &master);

            struct timeval tv = {};
            tv.tv_sec = 5;
            tv.tv_usec = 0;

            while(true) {
                rfds = master;
                int retval = select(socket_server+1, &rfds, nullptr, nullptr, &tv);
                if (retval > 0) {
                    if (FD_ISSET(STDIN_FILENO, &rfds)) {
                        if (fgets(buf, 35, stdin)) {
                            send(socket_server, buf, strlen(buf), 0);
                            std::cout << "Eu: " << buf << std::endl;
                        }
                    }
                    if (FD_ISSET(socket_server, &rfds)) {
                        ssize_t ret = recv(socket_server, buf, 4, 0);
                        std::cout << "João: " << buf << std::endl;
                    }
                } else {
                    std::cout << "retval failed: " << retval << std::endl;
                    std::cout << "socket disconnected: " << socket_server << std::endl;
                    close(socket_server);
                    break;
                }
            }*/
        }

        void sayHi () {

            std::unique_ptr<Common::Protocol> protocol (new Common::Protocol());
            protocol->type = htons(3);
            protocol->src  = htons(0);
            protocol->dest = htons(0);
            protocol->seq  = htons(0);

            send(this->getSocket(), protocol.get(), sizeof(*protocol), 0);
        }
    };

}

#endif //TCP_CHAT_MODULE_HPP
