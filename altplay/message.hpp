#pragma once
#include <string>

namespace irc_lib
{
	struct message_struct
	{
		std::string nick, hostmask, ident, message, argument, command;
		bool is_server_message;
	};
}
