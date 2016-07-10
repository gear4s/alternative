#include <cstdint>
#include <csignal>
#ifndef _WIN32
#include <sys/resource.h>
#include <sys/prctl.h>
#endif
#include "luaglue.h"

using namespace luabridge;
namespace altplay {
  namespace script {
    namespace lua {
      lua_State *L;
      
      std::list<irchook> _hooks;

      void hook(std::string command, LuaRef function)
      {
        irchook hook(L);

        hook.command = command;
        hook.function = function;

        _hooks.push_back(hook);
      }

      std::tuple<int, const char *> init() {
        auto quitter = [](int) { altplay::quit = true; };
        signal(SIGTERM, quitter);
        signal(SIGINT, quitter);
        L = luaL_newstate();
        luaL_openlibs(L);

        getGlobalNamespace(L)
          .beginClass<message_struct>("message_struct")
#define Data(n) addData(#n, &message_struct::n)
            .Data(nick)
            .Data(hostmask)
            .Data(ident)
            .Data(prefix)
            .Data(params)
            .Data(target)
            .Data(message)
            .Data(is_server_message)
#undef setProp
          .endClass()

          .beginNamespace("bot")
            .addFunction("hook", &hook)
            .addProperty("quit", +[] { return altplay::quit; }, +[](bool v) {
              if (altplay::quit && !v) luaL_error(L, "Cannot abort a quit");
              altplay::quit = v;
            })
          .endNamespace();

        if (!L) {
          return std::make_tuple(0, "");
        }
        if (luaL_loadfile(L, "script.lua")){
          return std::make_tuple(1, lua_tostring(L, -1));
        }
        lua_call(L, 0, 0);
        return std::make_tuple(2, "");
      }
      
      void callhook(std::string command, message_struct message) {
        if (_hooks.empty())
          return;

        for (std::list<irchook>::const_iterator itr = _hooks.begin(); itr != _hooks.end(); ++itr) {
          if (itr->command == command) {
            (itr->function)(message);
            break;
          }
        }
      }
    }
  }
}