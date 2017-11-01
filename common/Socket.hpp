//
// Created by tracksale on 10/31/17.
//

#ifndef TCP_CHAT_SOCKET_HPP
#define TCP_CHAT_SOCKET_HPP

#include <sys/socket.h>
#include <stdexcept>

#define LOCAL_HOST "127.0.0.1"

namespace Common {

    class Socket {

        int _socket;

    public:


    protected:

        Socket()
        {
            _socket = socket(AF_INET, SOCK_STREAM, 0);
            if (_socket < 0) {
                throw std::runtime_error("ERROR opening socket");
            }

        }

        int getSocket()
        {
            return _socket;
        }

    };
}

#endif //TCP_CHAT_SOCKET_HPP
