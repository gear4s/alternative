#include <regex>
#include "parser.hpp"

altplay::message_struct altplay::parser::handle_input(const std::string& str) const
{
	bool is_server_message{false};
	std::string nick{"null"}, ident{"null"}, message{"null"}, hostmask{"null"}, command{"null"}, argument{"null"};
	std::smatch match;
	std::regex client_regex{":(.*)!(.*)@([0-9.A-Za-z]*) ([A-Za-z]*) ([a-zA-Z0-9#&]*) :(.*)"},
		server_regex{":([A-Za-z.0-9]*) ([0-9A-Za-z]*) ([A-Za-z0-9]*) ([A-Za-z0-9 =@,+()#&:]*)"},
		notice_regex{"NOTICE AUTH :(.*)"};

	if (std::regex_search(str, match, client_regex))
	{
		nick = match[1];
		ident = match[2];
		hostmask = match[3];
		command = match[4];
		argument = match[5];
		message = match[6];
	}
	else if (std::regex_search(str, match, server_regex))
	{
		is_server_message = true;
		nick = match[1];
		command = match[2];
		argument = match[3];
		message = match[4];
	}
	else if (std::regex_search(str, match, notice_regex))
	{
		is_server_message = true;
		nick = "NOTICE AUTH";
		message = match[1];
	}
	else
	{
		throw std::runtime_error("unable to parse, please provide the developers"
			" with the server message that caused this.");
	}

	message_struct msg;

	msg.nick = nick;
	msg.hostmask = hostmask;
	msg.ident = ident;
	msg.message = message;
	msg.command = command;
	msg.argument = argument;
	msg.is_server_message = is_server_message;

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
