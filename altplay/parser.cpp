#include "parser.hpp"

irc_lib::message_struct irc_lib::parser::handle_input(const std::string& str)
{
	current_state_ = START;
	std::string nick, host, ident, message;

	for (auto iter = str.begin(); iter != str.end(); ++iter)
	{
		switch (current_state_)
		{
		case START:
			if (*iter == '.') current_state_ = SERVERMESSAGE;
			else if ( *iter == ':' ) break; // skip the first ':'
			else if ( *iter == '!' ) current_state_ = IDENT;
			else nick.push_back ( *iter );
			break;
		case SERVERMESSAGE:
			if (*iter == '\n') current_state_ = DONE_SERVER;
			nick.push_back(*iter);
			break;
		case MESSAGE:
			if (*iter == '\n') current_state_ = DONE_CLIENT;
			else message.push_back(*iter);
			break;
		case HOST:
			if (*iter == ' ') current_state_ = MESSAGE;
			else host.push_back(*iter);
			break;
		case IDENT:
			if (*iter == '@') current_state_ = HOST;
			else ident.push_back(*iter);
			break;
		case PARSE_ERROR:
			// TO DO: implement, agree upon error codes before!
			break;
		default:
			break;
		}
	}
	message_struct msg;

	if (current_state_ == SERVERMESSAGE)
	{
		msg.nick = "SERVER";
		msg.ident = "NULL";
		msg.hostmask = "NULL";
		msg.message = nick;
	}

	else
	{
		msg.nick = nick;
		msg.ident = ident;
		msg.hostmask = host;
		msg.message = message;
	}

	return msg;
}
