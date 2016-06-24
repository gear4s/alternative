#pragma once
#include <fstream>

namespace altplay
{
class logger
{
public:
    explicit logger ( std::string path );
    void add_entry ( const std::string& str );

private:
    std::fstream file_;
};
} // end of ns altplay
