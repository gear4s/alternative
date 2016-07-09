cmake_minimum_required(VERSION 3.5)
project(altplay)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL  "${CMAKE_CXX_FLAGS} -Os -fomit-frame-pointer")

set(SOURCE_FILES
        bot.cpp
        bot.hpp
        connection.cpp
        connection.hpp
        main.cpp
        message.hpp
        parser.cpp
        parser.hpp
        logger.cpp
        logger.hpp
        user.cpp
        user.hpp
        user_handler.cpp
        user_handler.hpp
        ./script/luaglue.cpp
        ./script/luaglue.h
        ./script/script.cpp
        ./script/script.h
        ./include/lua.h
        ./include/lua.hpp
        ./include/luaconf.h
        ./include/lualib.h
        ./include/lauxlib.h
        ./include/LuaBridge/LuaBridge.h)

if(DEBUG_ON)
    add_definitions(-DDEBUG_ON)
endif(DEBUG_ON)

include_directories("$ENV{HOME}/coding/cpp/gsl/include")
add_executable(altplay ${SOURCE_FILES})
target_link_libraries(altplay pthread)