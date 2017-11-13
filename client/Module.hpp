//
// Created by tracksale on 10/31/17.
//

#ifndef TCP_CHAT_MODULE_HPP
#define TCP_CHAT_MODULE_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <cstring>
#include <thread>
#include <sstream>
#include "../common/Socket.hpp"
#include "../common/Protocol.hpp"
#include "UserInterface.hpp"

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

            std::string connecting ("Conectando em ");
            connecting += LOCAL_HOST;
            connecting += ':';
            connecting += port.c_str();
            std::cout << connecting << std::endl;

            auto *sa_dst = (struct sockaddr *)&dst;
            while(connect(m_socket_, sa_dst, sizeof(dst)));
            this->sayHi();

            this->tcpSelect(m_socket_, [this](int event_mask, int socket_) {
                this->handleEvent(event_mask, socket_);
            });
        }


    private:

        uint16_t my_id_ = 0;
        uint16_t seq_counter = 0;

        void handleEvent(int event_mask, int socket_) {
            if ((event_mask & Common::Socket::EVENT_TYPE::KEYBOARD) == Common::Socket::EVENT_TYPE::KEYBOARD) {
                auto buf = (char*) malloc(405);
                if (fgets(buf, 405, stdin)) {
                    std::unique_ptr<Common::header_str> header_ (new Common::header_str);
                    std::shared_ptr<Common::Protocol> protocol (new Common::Protocol());

                    char* pch = strtok (buf," ");

                    switch (pch[0])
                    {
                        case 'M':
                        {
                            pch = strtok (nullptr, " ");
                            header_->type = Common::Protocol::MSG;
                            header_->dest = atoi(&pch[1]);
                            header_->seq = seq_counter;
                            header_->src = this->my_id_;

                            std::stringstream str_buf;

                            pch = strtok (nullptr, " ");
                            while (pch != NULL) {
                                str_buf << pch << " ";
                                pch = strtok (nullptr, " ");
                            }

                            std::string message_ = str_buf.str();

                            std::cout << "Eu (" << this->my_id_ << "): " << message_ << std::flush;

                            std::unique_ptr<Common::msg_str<char>> msg_(new Common::msg_str<char>);
                            msg_->C = static_cast<uint16_t>(message_.size());
                            strcpy(msg_->text, message_.c_str());

                            protocol->setHeader(header_.get());
                            protocol->setMsg(msg_.get());

                            this->tcpSend(socket_, protocol);

                            this->seq_counter++;
                        }
                            break;

                        case 'L':
                        {
                            header_->type = Common::Protocol::CREQ;
                            header_->dest = 0;
                            header_->seq = seq_counter;
                            header_->src = this->my_id_;

                            protocol->setHeader(header_.get());

                            this->tcpSend(socket_, protocol);
                        }
                            break;

                        default:
                            std::cout << pch[0] << " nao e uma opcao valida" << std::endl;
                    }
                }
            }
            if ((event_mask & Common::Socket::EVENT_TYPE::RECEIVE) == Common::Socket::EVENT_TYPE::RECEIVE) {
                std::shared_ptr<Common::Protocol> protocol_ (this->receive(socket_));
                Common::header_str* header_ = protocol_->getHeader();

                switch (header_->type)
                {
                    case Common::Protocol::TYPE::OK:
                    {
                        if(header_->seq == 0) {
                            this->my_id_ = header_->dest;
                            std::cout << "Conexao realizada com sucesso! Seu id e: "<< this->my_id_ << std::endl << std::endl;
                            std::cout << "Escolha uma das opcoes:" << std::endl;
                            std::cout << "\tL : ver lista de usuarios conectados" << std::endl;
                            std::cout << "\tM <id> <mensagem>: enviar mensagem para usuário. Id 0 envia para todos" << std::endl;
                            std::cout << "\tS : sair" << std::endl;
                        }
                    }
                        break;

                    case Common::Protocol::TYPE::ERRO:
                        break;

                    case Common::Protocol::TYPE::MSG:
                    {
                        Common::msg_str<char>* msg_tex;
                        protocol_->getMsg(&msg_tex);

                        std::cout << "Mensagem de " << protocol_->getHeader()->src << ": " << msg_tex->text << std::flush;
                    }
                        break;

                    case Common::Protocol::TYPE::CLIST:
                    {
                        Common::msg_str<uint16_t>*msg_num;
                        protocol_->getMsg(&msg_num);

                        int i = 0;
                        std::cout  << "Clientes disponíveis: " << std::flush;
                        for (; i < msg_num->C; i ++) {
                            std::cout << msg_num->text[i] << " ";
                        }
                        std::cout << std::endl;
                    }
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
