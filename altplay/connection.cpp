#include "connection.hpp"
#include <iostream>
#include <unordered_map>
#include "parser.hpp"


namespace altplay
{
connection::connection(asio::io_service &io_service_, bot_read_handler_t rh) : socket_{io_service_},
    bot_read_handler_{rh}
{
    std::unordered_map<std::string, std::string> config_map{parser::parse_config("config.conf")};
    asio::ip::tcp::resolver resolver{io_service_};
    asio::ip::tcp::resolver::query query{config_map.at("server_address"),config_map.at("server_port")};
    auto iter = resolver.resolve( query );
    connect( socket_, iter );
    do_read( );
}

// send raw data to the server
void connection::raw_send(std::string &content)
{
    content.append( "\r\n" );
    write( socket_, asio::buffer( content ) );
}


//asynchronous read handler, passes strings to the bot_read_handler_ (entry point for any bot)
void connection::read_handler(const asio::error_code &ec, size_t /*length*/)
{
    if ( !ec ) {
        std::string result_line;
        {
            std::lock_guard<std::mutex > lock( mutex_ ); // prevent iterator invalidation because of two concurrent reads
            std::istream is( &buffer_ );
            getline( is, result_line );
        }
        if ( regex_search( result_line, match_, std::regex( "PING (:[0-9A-Za-z.]*)" ) ) ) {
            std::string reply{"PONG "};
            reply.append( match_[1] );
            raw_send( reply );
        } else bot_read_handler_( result_line );
    }
    do_write( );
}

void connection::do_read( )
{
    if ( stop ) {
        socket_.close();
        return;
    }
    async_read_until( socket_,
                      buffer_,
                      "\r\n",
                      std::bind( &connection::read_handler, this, std::placeholders::_1, std::placeholders::_2 ) );
}


void connection::do_write( )
{
    std::lock_guard<std::mutex > lock( mutex_ );
    if ( !send_queue.empty( ) ) {
        raw_send( send_queue.front( ) );
        send_queue.pop_front( );
    }
    do_read( );
}

void connection::add_message(std::string &message)
{
    std::lock_guard<std::mutex > lock( mutex_ );
    send_queue.push_back( message );
}

void connection::shutdown()
{
    std::string quit_message{"QUIT :shutting down"};
    this->raw_send(quit_message);
    stop = true;
}

} // end of ns altplay
