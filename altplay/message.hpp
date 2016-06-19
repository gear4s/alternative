#pragma once
#include <string>

namespace altplay
{
    struct message_struct
    {
        std::string nick{"null"}, hostmask{"null"}, ident{"null"}, message{"null"}, argument{"null"}, command{"null"};
        bool is_server_message;
    };
} // end of ns altplay
