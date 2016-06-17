#include "Connection.hpp"
#include <iostream>

using namespace std;
using namespace asio;
using asio::ip::tcp;


connection::connection(asio::io_service& io_service_, string nick, string host): socket_{io_service_}, nick_{nick}, user_{host}
{
	tcp::resolver resolver{io_service_};
	tcp::resolver::query query{"irc.gamesurge.net", "6667"};
	auto iter = resolver.resolve(query);
	connect(socket_, iter);
#ifdef DEBUG_ON
	cout << "connected " << endl;
#endif
	reg_with_server();
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
		lock_guard<mutex> lock(mutex_);
		istream is(&buffer_);
		string result_line;
		getline(is, result_line);
		cout << result_line << endl;

		if (regex_search(result_line, match_, regex("PING (:[0-9A-Za-z.]*)")))
		{
			string reply{"PONG "};
			reply.append(match_[1]);
			cout << reply << endl;
			raw_send(reply);
		}

#ifdef DEBUG_ON
		else if (regex_search(result_line, match_, regex(":Marentis!alex@Marentis.agent.support PRIVMSG altbot :([0-9A-Za-z. :#]*)")))
		{
			string reply{match_[1]};
			send_queue.push_back(reply);
		}
#endif
	}
	do_write();
}

// generic do_read function, planned to accept various do_read handlers, provided by a bot
void connection::do_read()
{
	if (stop_server)
	{
		return;
	}
	async_read_until(socket_, buffer_, "\r\n", std::bind(&connection::read_handler, this, std::placeholders::_1, std::placeholders::_2));
}

// TO DO: add error handling, like when a certain nick is taken already and similar issues.
void connection::reg_with_server()
{
	string nick = "NICK " + nick_;
	string user = "USER " + user_;
	raw_send(user);
	this_thread::sleep_for(chrono::milliseconds(100));
	raw_send(nick);
}

// stop the bot
void connection::stop()
{
	this->stop_server = true;
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
