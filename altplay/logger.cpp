#include "logger.hpp"


altplay::logger::logger(std::string path)
{
#ifdef DEBUG_ON
    this->file_.rdbuf()->pubsetbuf(0, 0);
#endif
    this->file_.open(path, std::ios::app | std::ios::in | std::ios::out);
    if (!file_.is_open()) throw std::runtime_error("logger was unable to open the provided file.");
}

void altplay::logger::add_entry(const std::string& str)
{
    file_ << str.data() << std::endl;
}
