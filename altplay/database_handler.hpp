//
// Created by alex on 10.07.16.
//

#ifndef ALTPLAY_DATABASE_HANDLER_HPP
#define ALTPLAY_DATABASE_HANDLER_HPP

#include <mysql++/mysql++.h>
#include <regex>
#include "parser.hpp"

namespace altplay {
    class database_handler {
    public:
        database_handler();

        std::string escape_string(const std::string& str);
    private:
        mysqlpp::Connection con_;
        int port_;
        std::string host_, user_, password_, database_;
    };
}

#endif //ALTPLAY_DATABASE_HANDLER_HPP
