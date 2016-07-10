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
        database_handler() {
            try {
                auto config_map = parser::parse_config("config.conf");
                host_ = config_map.at("db_host");
                auto temp = config_map.at("db_port");
                try {
                    port_ = std::stoi(temp.data());
                    if ( port_ < 0 || port_ > 65535 ) {
                        throw std::runtime_error("Port is out of range");
                    }
                } catch ( const std::exception& e) {
                    throw std::runtime_error("Not a port number");
                }
                user_ = config_map.at("db_user");
                password_ = config_map.at("db_password");
                database_ = config_map.at("db_database");
                con_.connect(database_.data(), host_.data(), user_.data(), password_.data(), port_);
            } catch (const std::exception &e) {
                /* rethrow the exception, let the caller decide what to do when database connection fails. */
                throw;
            }

        }

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
