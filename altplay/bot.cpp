#include "bot.hpp"


bot::bot(asio::io_service& io_service_): con {io_service_, "altbot", "altbot altbot altbot :altbot", bind(&bot::read_handler, this, std::placeholders::_1)}
{

}

void bot::read_handler(const std::string& str)
{
	std::cout << str << std::endl;
}
