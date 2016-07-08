#include <iostream>
#include "bot.hpp"
#include "parser.hpp"
#include "message.hpp"
#include "script/script.h"


altplay::bot::bot(asio::io_service& io_service_) : con_ {io_service_, bind(&bot::read_handler, this, std::placeholders::_1)}, logger_ {"log.txt"}
{
	script::initScripts();
    std::unordered_map<std::string, std::string> config_map{parser::parse_config("config.conf")};
    nick_ = config_map.at("bot_nick");
    user_ = config_map.at("bot_user");
    reg_with_server();
}

void altplay::bot::read_handler(const std::string& str)
{
    try {
        logger_.add_entry(str);
        altplay::message_struct msg = parser::handle_input(str);
#ifdef DEBUG_ON
        if (msg.nick.compare("Marentis") == 0) {
            if (msg.message.compare("shutdown") == 0) {
                std::cout << "shutting down" << std::endl;
                con_.shutdown();
            }
            con_.add_message(msg.message);
        }
#endif
		script::lua::callhook(msg.command, msg);
        std::cout << str << std::endl;


    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << str << std::endl;
        logger_.add_entry("ERROR: " + str);
    }
}

// TO DO: add error handling, like when a certain nick is taken already and similar issues.
void altplay::bot::reg_with_server()
{
    std::string nick = "NICK " + nick_;
    std::string user = "USER " + user_;
    con_.add_message(user);
    con_.add_message(nick);
}
