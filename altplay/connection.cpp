#include "connection.hpp"
#include <iostream>
#include <unordered_map>
#include "parser.hpp"

using namespace std;
using namespace asio;
using ip::tcp;

namespace altplay
{
    connection::connection(asio::io_service &io_service_, bot_read_handler_t rh) : socket_{io_service_},
                                                                                   bot_read_handler_{rh}
    {
		std::unordered_map<std::string, std::string> config_map{parser::parse_config("config.conf")};
        tcp::resolver resolver{io_service_};
        tcp::resolver::query query{config_map.at("server_address"),config_map.at("server_port")};
        auto iter = resolver.resolve( query );
        connect( socket_, iter );
        do_read( );
    }

    // send raw data to the server
    void connection::raw_send(string &content)
    {
        content.append( "\r\n" );
        write( socket_, buffer( content ) );
    }


    //asynchronous read handler, passes strings to the bot_read_handler_ (entry point for any bot)
    void connection::read_handler(const asio::error_code &ec, size_t /*length*/)
    {
        if ( !ec ) {
            string result_line;
            {
                lock_guard< mutex > lock( mutex_ ); // prevent iterator invalidation because of two concurrent reads
                istream is( &buffer_ );
                getline( is, result_line );
            }
            if ( regex_search( result_line, match_, regex( "PING (:[0-9A-Za-z.]*)" ) ) ) {
                string reply{"PONG "};
                reply.append( match_[1] );
                raw_send( reply );
            }
            else bot_read_handler_( result_line );
        }
        do_write( );
    }

    void connection::do_read( )
    {
        async_read_until( socket_,
                          buffer_,
                          "\r\n",
                          std::bind( &connection::read_handler, this, std::placeholders::_1, std::placeholders::_2 ) );
    }


    void connection::do_write( )
    {
        lock_guard< mutex > lock( mutex_ );
        if ( !send_queue.empty( ) ) {
            raw_send( send_queue.front( ) );
            send_queue.pop_front( );
        }
        do_read( );
    }

    void connection::add_message(std::string &message)
    {
        lock_guard< mutex > lock( mutex_ );
        send_queue.push_back( message );
    }
} // end of ns altplay
