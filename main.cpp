#define DEBUG_ON
#include <asio.hpp>
#include <mutex>
#include <iostream>
#include <regex>
#include "Connection.hpp"

using namespace std;
using asio::ip::tcp;

int main()
{
	try
	{
		string cmdLine;
		asio::io_service io_service_;
		connection con(io_service_, "altbot", "altbot altbot altbot :altbot" );
		io_service_.run();
	}

	catch (const exception& e)
	{
		cout << e.what() << endl;
		getchar ( );
	}

	return 0;
}
