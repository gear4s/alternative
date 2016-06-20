#include <regex>
#include <iostream>
#include <fstream>
#include "parser.hpp"


altplay::message_struct altplay::parser::handle_input(const std::string& str)
{
    std::smatch match;
    message_struct msg;

    if (std::regex_search(str, match, std::regex(":([A-Za-z0-9!.~@\\[\\]-]*)\\s([A-Za-z0-9]*)\\s(.*)"))) {
        msg.prefix = match[1];
        msg.command = match[2];
        msg.params = match[3];

        if (std::regex_search(msg.prefix, match, std::regex("([A-Za-z0-9\\[\\]]*)!([A-Za-z0-9~]*)@([A-Za-z0-9.]*)"))) {
            msg.nick = match[1];
            msg.ident = match[2];
            msg.hostmask = match[3];
            std::regex_search(msg.params, match, std::regex{"([A-Za-z0-9\\[\\].#&!]*) :{0,1}(.*)"});
            msg.target = match[1];
            msg.message = match[2];
        } else {
            msg.is_server_message = true;
            msg.nick = msg.prefix;
        }
    } else {
        throw std::runtime_error("unable to parse, please provide the developers"
                                 " with the server message that caused this.");
    }
    return msg;
}

bool altplay::parser::compare_nicks(std::string nick1, std::string nick2)
{
    std::transform(nick1.begin(), nick1.end(), nick1.begin(), ::tolower);
    std::transform(nick2.begin(), nick2.end(), nick2.begin(), ::tolower);
    return (nick1.compare(nick2) == 0) ? true : false;
}

bool altplay::parser::compare_channel_names(std::string channel1, std::string channel2)
{
    std::transform(channel1.begin(), channel1.end(), channel1.begin(), ::tolower);
    std::transform(channel2.begin(), channel2.end(), channel2.begin(), ::tolower);
    return (channel1.compare(channel2) == 0) ? true : false;
}

const std::unordered_map<std::string, std::string> altplay::parser::parse_config(const std::string& path)
{
    std::fstream file{path, std::ios::in};
    std::string input;
    std::smatch match;
    std::unordered_map<std::string, std::string> config_map;
    int line_number{0};

    if (!file.is_open()) {
        throw std::runtime_error("Couldn't open config file.\n");
    }
    while (getline(file, input)) {
        ++line_number;
        if (std::regex_search(input, match, std::regex("([A-Z_a-z0-9]*) = \"([A-Za-z0-9._#!?:$ ]*)\""))) {
            config_map.insert(std::pair<std::string, std::string> {match[1], match[2]});
        } else {
            throw std::runtime_error("\nCould not read config at line: " + std::to_string(line_number));
        }
    }
    return config_map;
}
