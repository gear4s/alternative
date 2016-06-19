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
		bool compare_nicks(std::string nick1, std::string nick2);
		// returns true when channel names match, case insensitive
		bool compare_channel_names(std::string channel1, std::string channel2);
	};
} // end of ns irc_lib
