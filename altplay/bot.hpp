#pragma once
#include <asio.hpp>
#include "connection.hpp"
#include "parser.hpp"

namespace altplay
{
	class bot
	{
	public:
		bot(asio::io_service& io_service_);

		// entry point for this bot, supplied to a connection as a callback
		void read_handler(const std::string& str);
		void reg_with_server();

	private:
		altplay::connection con_;
		altplay::parser parser_;
		std::string nick_, user_;
	};
} // end of ns irc_lib
