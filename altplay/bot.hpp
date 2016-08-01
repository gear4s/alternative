#pragma once
#include <asio.hpp>
#include "connection.hpp"
#include "parser.hpp"
#include "logger.hpp"

namespace altplay
{
	class bot {
	public:
		explicit bot(asio::io_service& io_service_);

		// entry point for this bot, supplied to a connection as a callback
		void read_handler(const std::string& str);
    void reg_with_server();

    void send_raw(std::string, ...);
    const char *strformat(std::string, va_list);

	private:
		altplay::connection con_;
		altplay::logger logger_;
		std::string nick_, user_;
	};

  extern bool quit;
} // end of ns altplay
