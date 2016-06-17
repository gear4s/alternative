#pragma once
#include <asio.hpp>
#include "connection.hpp"

class bot
{
public:
	bot(asio::io_service& io_service_);

	// entry point for this bot, supplied to a connection as a callback
	void read_handler(const std::string& str);
	void reg_with_server ( );

private:
	irc_lib::connection con;
	std::string nick_;
	std::string user_;
};
