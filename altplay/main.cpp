#define _GLIBCXX_USE_NANOSLEEP
#include <asio.hpp>
#include <iostream>
#include <clocale>
#include <thread>
#include "bot.hpp"
#include "later.h"

using namespace std;
using asio::ip::tcp;

namespace altplay {
  bot *botinstance;
}

bool quit = false;
int main( )
{
    std::thread tT([]() {
      while (!quit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        altplay::script::lua::later::servermillis += 1;
        altplay::script::lua::later::check();
      }
    });

    try {
        std::setlocale ( LC_ALL, "en_US.UTF-8" );
        asio::io_service io_service_;
        asio::signal_set signals(io_service_, SIGINT, SIGTERM);
        signals.async_wait(std::bind(&asio::io_service::stop, &io_service_));
        altplay::bot *bot = new altplay::bot( io_service_ );
        altplay::botinstance = bot;
        io_service_.run();
    } catch ( const exception &e ) {
        cout << e.what( ) << endl;
        getchar( );
    }
    
    quit = true;

    //detach threadTimer from main thread
    if (tT.joinable())
      //main is blocked until tT is not finished
      tT.join();

    return 0;
}
