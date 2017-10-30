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

        static std::array<int, 7> _valid_msg_types = {1,2,3,4,5,6,7};

    public:

        static bool checkValidMsg(unsigned short &type)
        {
            return std::find(
                    Messages::_valid_msg_types.begin(),
                    Messages::_valid_msg_types.end(),
                    type) != std::end(Messages::_valid_msg_types);
        }
    };
}


#endif //TCP_CHAT_MESSAGES_HPP
