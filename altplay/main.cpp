#include <asio.hpp>
#include <iostream>
#include <clocale>
#include "bot.hpp"

using namespace std;
using asio::ip::tcp;

int main( )
{
    try {
        std::setlocale ( LC_ALL, "en_US.UTF-8" );
        asio::io_service io_service_;
        altplay::bot bot( io_service_ );
        io_service_.run( );
    }

    catch ( const exception &e ) {
        cout << e.what( ) << endl;
        getchar( );
    }

    return 0;
}
