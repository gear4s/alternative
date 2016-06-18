cmake_minimum_required(VERSION 3.5)
project(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(".")
set(SOURCE_FILES main.cpp bot.cpp bot.hpp connection.cpp connection.hpp message.hpp parser.cpp parser.hpp)
add_executable(. ${SOURCE_FILES})