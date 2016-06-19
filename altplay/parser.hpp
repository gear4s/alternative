#pragma once
#include "message.hpp"

namespace irc_lib
{
	class parser
	{
	public:
		// strong error guarantee, won't change the original string and will raise an exception
		message_struct handle_input(const std::string& str) const;
		// returns true when nicks match, case insensitive
		// returns true when channel names match, case insensitive
	};
} // end of ns irc_lib
