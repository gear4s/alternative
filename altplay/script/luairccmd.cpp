#include "bot.hpp"
#include "script.h"
#include <iostream>

namespace altplay {
  extern bot *botinstance;
  namespace script {
    namespace lua {
      using namespace luabridge;
      void bindIrcToLua(lua_State *L) {
        getGlobalNamespace(L)
          .beginNamespace("irc")
            .addFunction("msg", +[](const char *chan, const char *msg) {
              botinstance->send_raw("PRIVMSG #%s :%s", chan, msg);
            })
            .addFunction("privmsg", +[](const char *who, const char *msg) {
              botinstance->send_raw("PRIVMSG %s :%s", who, msg);
            })
            .addFunction("notice", +[](const char *where, const char *msg) {
              botinstance->send_raw("PRIVMSG %s :%s", where, msg);
            })
            .addFunction("join", +[](const char *chan, const char *passes = "") {
              botinstance->send_raw("JOIN %s %s", chan, passes);
            })
            .addFunction("nick", +[](const char *what) {
              botinstance->send_raw("NICK %s", what);
            })
          .endNamespace();
      }
    }
  }
}