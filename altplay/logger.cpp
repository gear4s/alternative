#include "logger.hpp"


altplay::logger::logger(std::string path)
{
	this->file_.open(path, std::ios::app | std::ios::in | std::ios::out);
	if (!file_.is_open()) throw std::runtime_error("Unable to open the file.");
}

void altplay::logger::add_entry(const std::string& str)
{
	file_ << str.data() << std::endl;
}
