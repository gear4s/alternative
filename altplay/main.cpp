#include <asio.hpp>
#include <iostream>
#include <clocale>
#include "bot.hpp"

using namespace std;
using asio::ip::tcp;

namespace altplay {
  bot *botinstance;
}

int main( )
{
    try {
        std::setlocale ( LC_ALL, "en_US.UTF-8" );
        asio::io_service io_service_;
        asio::signal_set signals(io_service_, SIGINT, SIGTERM);
        signals.async_wait(std::bind(&asio::io_service::stop, &io_service_));
        altplay::bot *bot = new altplay::bot( io_service_ );
        altplay::botinstance = bot;
        io_service_.run( );
    }

    catch ( const exception &e ) {
        cout << e.what( ) << endl;
        getchar( );
    }

    return 0;
}
