#include <regex>
#include "parser.hpp"

irc_lib::message_struct irc_lib::parser::handle_input(const std::string& str) const
{
	bool is_server_message{false};
	std::string nick{"null"}, ident{"null"}, message{"null"}, hostmask{"null"}, command{"null"}, argument{"null"};
	std::smatch match;
	std::regex client_regex{":(.*)!(.*)@([0-9.A-Za-z]*) ([A-Za-z]*) ([a-zA-Z0-9]*) :(.*)"},
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
