#pragma once
#include "message.hpp"

namespace irc_lib
{
	class parser
	{
	public:
		// strong error guarantee, won't change the original string and will raise an exception
		// parses the raw string via a finite state machine and distinguishes between server and client messages
		message_struct handle_input(const std::string& str);
	};
} // end of ns irc_lib
