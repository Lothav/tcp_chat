//
// Created by tracksale on 10/31/17.
//

#ifndef TCP_CHAT_MODULE_HPP
#define TCP_CHAT_MODULE_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include "../common/Socket.hpp"

#define TC_MAX_REQUESTS 5

namespace Server {

    class Module : Common::Socket
    {

    public:
        explicit Module(std::string port)
        {
            int size = 1;
            int _socket_server = this->getSocket();
            if (setsockopt(_socket_server, SOL_SOCKET, SO_REUSEADDR, &size, sizeof(int)) < 0)
                printf("setsockopt(SO_REUSEADDR) failed");

            struct sockaddr_in 	serv_addr = {};
            serv_addr.sin_family 		= AF_INET;
            serv_addr.sin_port   		= htons(static_cast<uint16_t>(std::stoi(port.c_str())));
            inet_aton(LOCAL_HOST, &serv_addr.sin_addr);

            while (bind(_socket_server, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {}

            listen(this->getSocket(), TC_MAX_REQUESTS);

            struct sockaddr_in 	cli_addr = {};
            socklen_t clilen = sizeof(cli_addr);

            int _socket_client = accept(_socket_server, (struct sockaddr *)&cli_addr, &clilen);
        }

    };

}

#endif //TCP_CHAT_MODULE_HPP
