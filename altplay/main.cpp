#include <asio.hpp>
#include <iostream>
#include "connection.hpp"
#include "bot.hpp"

using namespace std;
using asio::ip::tcp;

int main( )
{
    try {
        asio::io_service io_service_;
        bot bot( io_service_ );
        io_service_.run( );
    }

    catch ( const exception &e ) {
        cout << e.what( ) << endl;
        getchar( );
    }

    return 0;
}
