#include <regex>
#include "parser.hpp"
#include <iostream>

altplay::message_struct altplay::parser::handle_input(const std::string& str) const
{
	std::smatch match;
	message_struct msg;

	if (std::regex_search(str, match, std::regex(":([A-Za-z0-9!.~@\\[\\]-]*)\\s([A-Za-z0-9]*)\\s(.*)")))
	{
		msg.prefix = match[1];
		msg.command = match[2];
		msg.params = match[3];

		if (std::regex_search(msg.prefix, match, std::regex("([A-Za-z0-9\\[\\]]*)!([A-Za-z0-9~]*)@([A-Za-z0-9.]*)")))
		{
			msg.nick = match[1];
			msg.ident = match[2];
			msg.hostmask = match[3];
			std::regex_search(msg.params, match, std::regex{"([A-Za-z0-9\\[\\].#&!]*) :{0,1}(.*)"});
			msg.target = match[1];
			msg.message = match[2];
		}
		else
		{
			msg.is_server_message = true;
			msg.nick = msg.prefix;
		}
	}
	else
	{
		throw std::runtime_error("unable to parse, please provide the developers"
			" with the server message that caused this.");
	}
	return msg;
}

bool altplay::parser::compare_nicks(std::string nick1, std::string nick2)
{
	std::transform(nick1.begin(), nick1.end(), nick1.begin(), ::tolower);
	std::transform(nick2.begin(), nick2.end(), nick2.begin(), ::tolower);
	return (nick1.compare(nick2) == 0) ? true : false;
}

bool altplay::parser::compare_channel_names(std::string channel1, std::string channel2)
{
	std::transform(channel1.begin(), channel1.end(), channel1.begin(), ::tolower);
	std::transform(channel2.begin(), channel2.end(), channel2.begin(), ::tolower);
	return (channel1.compare(channel2) == 0) ? true : false;
}
