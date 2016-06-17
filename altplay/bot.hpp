#pragma once
#include <string>
#include <asio.hpp>
#include "connection.hpp"
#include <iostream>

class bot
{
public:
	bot(asio::io_service& io_service_);

	void read_handler(const std::string& str);
	void reg_with_server ( );

private:
	irc_lib::connection con;
	std::string nick_;
	std::string user_;
};
