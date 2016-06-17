#pragma once
#include <asio.hpp>
#include <mutex>
#include <regex>
#include <deque>
#define DEBUG_ON

/*
A basic connection to an irc server with a keep alive implementation included.
Designed with an asynchronous do_read handler
Further plans:
1) use function pointers for do_read handlers
2) keep this class as simple as possible
3) bots will utilize this connection to add functionality
*/

class connection : public std::enable_shared_from_this<connection>
{
public:
	connection(asio::io_service& io_service_, std::string nick, std::string user);
	void read_handler(const asio::error_code& ec, size_t length);
	void do_read();
	void reg_with_server();
	void stop();
	void do_write();
	void addMessage(std::string& message);

private:
	void raw_send(std::string& content);
	std::deque<std::string> send_queue;
	bool stop_server{false};
	std::smatch match_;
	std::mutex mutex_;
	asio::streambuf buffer_;
	asio::ip::tcp::socket socket_;
	std::thread t1;
	std::string nick_;
	std::string user_;
};
