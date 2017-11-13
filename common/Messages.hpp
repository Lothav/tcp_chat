#ifndef TCP_CHAT_MESSAGES_HPP
#define TCP_CHAT_MESSAGES_HPP

#include <array>
#include <algorithm>

namespace Common {

    class Messages {

        enum MSG_TYPE : unsigned short {
            OK    = 1,
            ERRO  = 2,
            OI    = 3,
            FLW   = 4,
            MSG   = 5,
            CREQ  = 6,
            CLIST = 7
        } ;

    private:

        Messages() {};
        ~Messages() {};
    };
}


#endif //TCP_CHAT_MESSAGES_HPP
