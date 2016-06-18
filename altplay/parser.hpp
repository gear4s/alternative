#pragma once
#include <regex>
#include "message.hpp"

namespace irc_lib
{
	enum parser_states_enum
	{ START, SERVERMESSAGE, MESSAGE, HOST, IDENT, DONE_CLIENT, DONE_SERVER, PARSE_ERROR };

	class parser
	{
	public:
		// strong error guarantee, won't change the original string and will raise an exception
		message_struct handle_input(const std::string& str);

	private:
		parser_states_enum current_state_{START};
	};
} // end of ns irc_lib
