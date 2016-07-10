//
// Created by alex on 10.07.16.
//

#include "database_handler.hpp"

altplay::database_handler::database_handler() {
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