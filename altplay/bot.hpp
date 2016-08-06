#pragma once
#include <asio.hpp>
#include "connection.hpp"
#include "parser.hpp"
#include "logger.hpp"

namespace altplay
{
  extern bool quit;
	class bot {
	public:
		explicit bot(asio::io_service& io_service_);

		// entry point for this bot, supplied to a connection as a callback
		void read_handler(const std::string& str);
		void reg_with_server();

	private:
		altplay::connection con_;
		altplay::logger logger_;
		std::string nick_, user_;
        std::unordered_map<std::string, std::string> user_map_;
	};
} // end of ns altplay
