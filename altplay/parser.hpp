#pragma once
#include <unordered_map>
#include "message.hpp"

namespace altplay
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
		// parses a config file and returns a map with key <=> val 
		const std::unordered_map<std::string, std::string> parse_config(const std::string path) const;
	};
} // end of ns altplay
