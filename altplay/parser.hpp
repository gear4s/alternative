#pragma once
#include <regex>
#include "message.hpp"

namespace irc_lib
{
	class parser
	{
	public:
		// strong error guarantee, won't change the original string and will raise an exception
		// parses the raw string via a finite state machine and distinguishes between server and client messages
		message_struct handle_input(const std::string& str);

	private:
		enum parser_states { START, SERVERMESSAGE, MESSAGE, HOST, IDENT, DONE_CLIENT, DONE_SERVER, PARSE_ERROR };
		parser_states current_state_{START};
	};
} // end of ns irc_lib
