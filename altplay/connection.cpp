#include "Connection.hpp"
#include <iostream>

using namespace std;
using namespace asio;
using asio::ip::tcp;

namespace irc_lib
{
	connection::connection(asio::io_service& io_service_, bot_read_handler_t rh) : socket_{io_service_}, bot_read_handler_ {rh}
	{
		tcp::resolver resolver{io_service_};
		tcp::resolver::query query{"irc.gamesurge.net", "6667"};
		auto iter = resolver.resolve(query);
		connect(socket_, iter);
#ifdef DEBUG_ON
		cout << "connected " << endl;
#endif
		do_read();
	}

	// send raw data to the server
	void connection::raw_send(string& content)
	{
		content.append("\r\n");
		write(socket_, buffer(content));
	}

	// asynchronous default do_read handler with a ping/pong implementation
	void connection::read_handler(const asio::error_code& ec, size_t length)
	{
		if (!ec)
		{
			string result_line;
			{
				lock_guard<mutex> lock(mutex_); // prevent iterator invalidation because of two concurrent reads
				istream is(&buffer_);
				getline(is, result_line);
			}
			if (regex_search(result_line, match_, regex("PING (:[0-9A-Za-z.]*)")))
			{
				string reply{"PONG "};
				reply.append(match_[1]);
				raw_send(reply);
			}
			else bot_read_handler_(result_line);
		}
		do_write();
	}

	// generic do_read function, planned to accept various do_read handlers, provided by a bot
	void connection::do_read()
	{
		async_read_until(socket_, buffer_, "\r\n", std::bind(&connection::read_handler, this, std::placeholders::_1, std::placeholders::_2));
	}


	void connection::do_write()
	{
		if (!send_queue.empty())
		{
			raw_send(send_queue.front());
			send_queue.pop_front();
		}
		do_read();
	}

	void connection::addMessage(std::string& message)
	{
		lock_guard<mutex> lock(mutex_);
		send_queue.push_back(message);
	}
} // end of ns irc_lib
