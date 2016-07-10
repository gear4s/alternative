#pragma once
#include "LuaBridge/LuaBridge.h"
#include "message.hpp"
#include "bot.hpp"
#include <list>
#include <tuple>

namespace altplay {
  namespace script {
    namespace lua {
      extern lua_State *L;
      extern std::tuple<int, const char *> init();
      extern void callhook(std::string, message_struct);

      struct irchook
      {
        irchook(lua_State *L) : function(L) {};

        std::string command;
        luabridge::LuaRef function;
      };
      extern std::list<irchook> _hooks;
    }
  }
}