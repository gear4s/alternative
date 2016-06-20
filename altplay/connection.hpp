#pragma once
#include <asio.hpp>
#include <mutex>
#include <regex>
#include <deque>

/*
A basic connection to an irc server with a keep alive implementation included.
Designed with an asynchronous do_read handler
Further plans:
1) use function pointers for do_read handlers
2) keep this class as simple as possible
3) bots will utilize this connection to add functionality
*/

namespace altplay
{
// register a bot's read handler as type bot_read_handler_t (std::function<void(const std::string&>)
using bot_read_handler_t = std::function< void(const std::string &) >;

class connection: public std::enable_shared_from_this< connection >
{
public:
    connection(asio::io_service &io_service_, bot_read_handler_t rh);
    void add_message(std::string &message);

private:

    void raw_send(std::string &content);
    void do_read ( );
    void do_write ( );
    void read_handler ( const asio::error_code &ec, size_t length );

    std::deque< std::string > send_queue;
    std::smatch match_;
    std::mutex mutex_;
    asio::streambuf buffer_;
    asio::ip::tcp::socket socket_;
    bot_read_handler_t bot_read_handler_; // accepts a bot's read handler
};
} // end of ns altplay
