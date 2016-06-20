#include <iostream>
#include "bot.hpp"
#include "parser.hpp"
#include "message.hpp"

using namespace std;

altplay::bot::bot(asio::io_service& io_service_) : con_ {io_service_, bind(&bot::read_handler, this, placeholders::_1)}, logger_ {"log.txt"}
{
	std::unordered_map<std::string, std::string> config_map{parser_.parse_config("config.conf")};
	nick_ = config_map.at("bot_nick");
	user_ = config_map.at("bot_user");
	reg_with_server();
}

void altplay::bot::read_handler(const string& str)
{
	try
	{
		logger_.add_entry(str);
		altplay::message_struct msg = parser_.handle_input(str);
#ifdef DEBUG_ON
		if (msg.nick.compare("Marentis") == 0)
		{
			con_.add_message(msg.message);
		}
#endif
		cout << str << endl;
	}
	catch (const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << str << endl;
		logger_.add_entry("ERROR: " + str);
	}
}

// TO DO: add error handling, like when a certain nick is taken already and similar issues.
void altplay::bot::reg_with_server()
{
	string nick = "NICK " + nick_;
	string user = "USER " + user_;
	con_.add_message(user);
	con_.add_message(nick);
}
