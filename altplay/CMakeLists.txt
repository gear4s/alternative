cmake_minimum_required(VERSION 3.5)
project(altplay)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Wpedantic -fpermissive")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -Os -fomit-frame-pointer")

set(SOURCE_FILES
        bot.cpp
        bot.hpp
        connection.cpp
        connection.hpp
        main.cpp
        message.hpp
        parser.cpp
        parser.hpp
        later.cpp
        later.h
        logger.cpp
        logger.hpp
        ./script/luaglue.cpp
        ./script/luaglue.h
        ./script/luairccmd.cpp
        ./script/luapcall.cpp
        ./script/luapcall.h
        ./script/script.cpp
        ./script/script.h
        ./include/lua.h
        ./include/lua.hpp
        ./include/luaconf.h
        ./include/lualib.h
        ./include/lauxlib.h
        ./include/LuaBridge/LuaBridge.h)

include_directories(${altplay_SOURCE_DIR}/ ${altplay_SOURCE_DIR}/include)

if (DEBUG_ON)
    add_definitions(-DDEBUG_ON)
endif (DEBUG_ON)

add_definitions(-DASIO_STANDALONE -DMYSQLPP_MYSQL_HEADERS_BURIED)

include_directories("$ENV{HOME}/coding/cpp/gsl/include")
add_executable(altplay ${SOURCE_FILES})
target_link_libraries(altplay pthread lua mysqlpp)