#include <iostream>
#include "bot.hpp"
#include "parser.hpp"
#include "message.hpp"

using namespace std;

bot::bot(asio::io_service &io_service_) : con_{io_service_, bind( &bot::read_handler, this, placeholders::_1 )}
{
    nick_ = "altbot";
    user_ = "altbot altbot altbot :altbot";
    reg_with_server( );
}

void bot::read_handler(const string &str)
{
    try {
        irc_lib::message_struct msg = parser_.handle_input( str );
        cout << msg.nick << ": " << msg.message << endl;
    } catch ( const std::exception &e ) {
        cerr << e.what( ) << endl;
    }
}

// TO DO: add error handling, like when a certain nick is taken already and similar issues.
void bot::reg_with_server( )
{
    string nick = "NICK " + nick_;
    string user = "USER " + user_;
    con_.add_message( user );
    con_.add_message( nick );
}
