#include "bot.hpp"
#include <iostream>
using namespace std;

bot::bot(asio::io_service& io_service_): con {io_service_, bind(&bot::read_handler, this, placeholders::_1)}
{
	nick_ = "altbot";
	user_ = "altbot altbot altbot :altbot";
	reg_with_server();
}

void bot::read_handler(const string& str)
{
	smatch match;
	if (regex_search(str, match, regex(":Marentis!alex@Marentis.agent.support PRIVMSG altbot :([A-Za-z0-9. #:/]*)")))
	{
		string reply{match[1]};
		con.add_message(reply);
	}

	cout << str << endl;
}

// TO DO: add error handling, like when a certain nick is taken already and similar issues.
void bot::reg_with_server()
{
	string nick = "NICK " + nick_;
	string user = "USER " + user_;
	con.add_message(user);
	con.add_message(nick);
}
