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
            int _socket_server = this->getSocket();
            if (setsockopt(_socket_server, SOL_SOCKET, SO_REUSEADDR, &size, sizeof(int)) < 0)
                printf("setsockopt(SO_REUSEADDR) failed");

            struct sockaddr_in serv_addr = {};
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port   = htons(static_cast<uint16_t>(std::atoi(port)));
            inet_aton(LOCAL_HOST, &serv_addr.sin_addr);

            while (bind(_socket_server, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0);

            listen(_socket_server, TC_MAX_REQUESTS);

            struct sockaddr_in 	cli_addr = {};
            socklen_t clilen = sizeof(cli_addr);

            int _socket_client = accept(_socket_server, (struct sockaddr *)&cli_addr, &clilen);
            this->receive(_socket_client);
            /*
            int cont = 30;
            while (cont--) {
                char buffer[4] = "ser";
                send(_socket_client, &buffer, strlen(buffer), 0);
                sleep(10);
            }*/
        }

        void receive (int socket)
        {
            size_t buffer_size = sizeof(Common::Protocol) + 400 + 2;
            std::unique_ptr<char> buffer ((char *)malloc(buffer_size));
            ssize_t recv_size = recv(socket, buffer.get(), buffer_size, 0);
            if (recv_size > 0) {
                std::unique_ptr<Common::Protocol> protocol( new Common::Protocol() );

                std::memcpy(protocol.get(), buffer.get(), sizeof(Common::Protocol));
                protocol->toHostOrder();

                std::cout << protocol->type << " " << protocol->src << " " << protocol->dest << " " << protocol->seq << " "  << std::endl;
            } else {
                throw "asd";
            }
        }
    };
}

#endif //TCP_CHAT_MODULE_HPP
