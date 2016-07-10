//
// Created by alex on 10.07.16.
//

#ifndef ALTPLAY_DATABASE_HANDLER_HPP
#define ALTPLAY_DATABASE_HANDLER_HPP

#include <mysql++/mysql++.h>
#include "parser.hpp"

namespace altplay {
    class database_handler {
    public:
        database_handler();

    private:
        mysqlpp::Connection con_;
        int port_;
        std::string host_;
        std::string user_;
        std::string password_;
        std::string database_;
    };
}

#endif //ALTPLAY_DATABASE_HANDLER_HPP
