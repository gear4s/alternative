#pragma once
#include <string>

namespace irc_lib
{
    struct message_struct
    {
        std::string nick{"null"}, hostmask{"null"}, ident{"null"}, message{"null"}, argument{"null"}, command{"null"};
        bool is_server_message;
    };
}
