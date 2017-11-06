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

        uint16_t my_id_ = 0;
        uint16_t seq_counter = 0;

        void handleEvent(int event_mask, int socket_) {
            if ((event_mask & Common::Socket::EVENT_TYPE::KEYBOARD) == Common::Socket::EVENT_TYPE::KEYBOARD) {
                char buf[35];
                if (fgets(buf, 35, stdin)) {
                    std::cout << "Eu: " << buf << std::endl;

                    auto* header_ = new Common::header_str;
                    header_->type = Common::Protocol::MSG;
                    header_->dest = 0;
                    header_->seq  = seq_counter;
                    header_->src  = this->my_id_;

                    auto* msg_ = new Common::msg_str;
                    msg_->C    = static_cast<uint16_t>(strlen(buf));
                    strcpy(msg_->msg, buf);

                    std::unique_ptr<Common::Protocol> protocol (new Common::Protocol());
                    protocol->setHeader(header_);
                    protocol->setMsg(msg_);

                    this->tcpSend(socket_, protocol.get());

                    this->seq_counter++;
                }
            }
            if ((event_mask & Common::Socket::EVENT_TYPE::RECEIVE) == Common::Socket::EVENT_TYPE::RECEIVE) {
                Common::Protocol* protocol_ = this->receive(socket_);
                Common::header_str* header_ = protocol_->getHeader();
                std::cout<< "type: " << header_->type << std::endl;
                switch (header_->type)
                {
                    case Common::Protocol::TYPE::OK:
                        this->my_id_ = header_->dest;
                        break;
                    default:
                        break;
                }
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
            this->seq_counter++;
        }
    };

}

#endif //TCP_CHAT_MODULE_HPP
