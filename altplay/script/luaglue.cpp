#include "bot.hpp"
#include "irctypes.h"
#include "script.h"
#include "connection.hpp"
#include <iostream>
#include <sstream>
#include "later.h"

using namespace luabridge;
namespace altplay {
  extern  bot *botinstance;

  namespace script {
    namespace lua {
      lua_State *L;

      std::list<irchook> _hooks;

      void hook(std::string command, LuaRef function) {
        irchook hook(L);

        hook.command = command;
        hook.function = function;

        _hooks.push_back(hook);
      }

      void hook(int command, LuaRef function) {
        irchook hook(L);

        hook.icommand = command;
        hook.function = function;

        _hooks.push_back(hook);
      }

      void callhook(std::string reply, message_struct message) {
        if (_hooks.empty())
          return;

        for (std::list<irchook>::const_iterator itr = _hooks.begin(); itr != _hooks.end(); ++itr) {
          if (itr->command == reply) {
            (itr->function)(message);
            break;
          }
        }
      }

      void callhook(int reply, message_struct message) {
        if (_hooks.empty())
          return;

        for (std::list<irchook>::const_iterator itr = _hooks.begin(); itr != _hooks.end(); ++itr) {
          if (itr->icommand == reply) {
            (itr->function)(message);
            break;
          }
        }
      }

      auto init() -> std::tuple<int, const char *> {
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
            .addCFunction("hook", [](lua_State* L)  -> int {
              int top = lua_gettop(L);
              if (top == 2) {
                if (lua_isnumber(L, 1)) {
                  printf("it's a %g\n", lua_tonumber(L, 1));
                  hook(lua_tonumber(L, 1), LuaRef(L).fromStack(L, 2));
                }
                else if (lua_isstring(L, 1)) {
                  printf("it's a %s\n", lua_tostring(L, 1));
                  hook(lua_tostring(L, 1), LuaRef(L).fromStack(L, 2));
                }
                else luaL_error(L, "first argument for hook invalid; expected string or number.");
              }
              return 0;
            })
            .addCFunction("send", [](lua_State *L) -> int {
              if (lua_gettop(L) == 1) {
                if (lua_isstring(L, 1)) {
                  botinstance->send_raw(lua_tostring(L, 1));
                }
              }
              return 0;
            })
            .beginClass<later::latertoken>("latertoken")
            .endClass()
            .addFunction("later", +[](lua_State* L) { return later::newlater(L); })
            .addFunction("cancel", later::cancel)
            .addProperty("quit", +[] { return altplay::quit; }, +[](bool v) {
              if (altplay::quit && !v) luaL_error(L, "Cannot abort a quit");
              altplay::quit = v;
            })

            // bot control functions here
            .addFunction("rename", +[](const char *newnick) {
              botinstance->send_raw("NICK %s", newnick);
            })
          .endNamespace()
          .beginNamespace("string")
            // implement string.split in core
            .addCFunction("split", +[](lua_State *L) -> int {
              if(lua_gettop(L) == 2) {
                const char *s = lua_tostring(L,1);
                const char *token = lua_tostring(L, 2);
                
                int i = 0; 
                lua_newtable(L);
                char* pch = strtok(s, token);
                lua_newtable(L);
                lua_pushnumber(L,i);
                lua_pushstring(L,pch);
                lua_settable(L,-3);
                while(pch != NULL) {i++;
                  pch = strtok(NULL,token);
                  lua_pushnumber(L,i);
                  lua_pushstring(L,pch);
                  lua_settable(L,-3);
                  i++;
                }
                
                return 1;
              }
            })
          .endNamespace();
#define addEnum(n)    lua_pushliteral(L, #n); lua_pushnumber(L, n); lua_rawset(L, -3)
        lua_newtable(L);
        lua_pushliteral(L, "reply");
        lua_newtable(L);
          addEnum(WELCOME); addEnum(YOURHOST); addEnum(CREATED); addEnum(MYINFO); addEnum(BOUNCE);
          addEnum(TRACELINK); addEnum(TRACECONNECTING); addEnum(TRACEHANDSHAKE); addEnum(TRACEUNKNOWN);
          addEnum(TRACEOPERATOR); addEnum(TRACEUSER); addEnum(TRACESERVER); addEnum(TRACESERVICE);
          addEnum(TRACENEWTYPE); addEnum(TRACECLASS); addEnum(TRACERECONNECT); addEnum(STATSLINKINFO);
          addEnum(STATSCOMMANDS); addEnum(ENDOFSTATS); addEnum(UMODEIS); addEnum(SERVLIST);
          addEnum(SERVLISTEND); addEnum(STATSUPTIME); addEnum(STATSONLINE); addEnum(LUSERCLIENT);
          addEnum(LUSEROP); addEnum(LUSERUNKNOWN); addEnum(LUSERCHANNELS); addEnum(LUSERME); addEnum(ADMINME);
          addEnum(ADMINLOC1); addEnum(ADMINLOC2); addEnum(ADMINEMAIL); addEnum(TRACELOG); addEnum(TRACEEND);
          addEnum(TRYAGAIN); addEnum(AWAY); addEnum(USERHOST); addEnum(ISON); addEnum(UNAWAY);
          addEnum(NOWAWAY); addEnum(WHOISUSER); addEnum(WHOISSERVER); addEnum(WHOISOPERATOR);
          addEnum(WHOWASUSER); addEnum(ENDOFWHO); addEnum(WHOISIDLE); addEnum(ENDOFWHOIS);
          addEnum(WHOISCHANNELS); addEnum(LISTSTART); addEnum(LIST); addEnum(LISTEND); addEnum(CHANNELMODEIS);
          addEnum(UNIQOPIS); addEnum(NOTOPIC); addEnum(TOPIC); addEnum(INVITING); addEnum(SUMMONING);
          addEnum(INVITELIST); addEnum(ENDOFINVITELIST); addEnum(EXCEPTLIST); addEnum(ENDOFEXCEPTLIST);
          addEnum(VERSION); addEnum(WHOREPLY); addEnum(NAMREPLY); addEnum(LINKS); addEnum(ENDOFLINKS);
          addEnum(ENDOFNAMES); addEnum(BANLIST); addEnum(ENDOFBANLIST); addEnum(ENDOFWHOWAS); addEnum(INFO);
          addEnum(MOTD); addEnum(ENDOFINFO); addEnum(MOTDSTART); addEnum(ENDOFMOTD); addEnum(YOUREOPER);
          addEnum(REHASHING); addEnum(YOURESERVICE); addEnum(TIME); addEnum(USERSSTART); addEnum(USERS);
        addEnum(ENDOFUSERS); addEnum(NOUSERS);
        lua_settable(L, -3);

        lua_pushliteral(L, "error");
        lua_newtable(L);
          addEnum(NOSUCHNICK); addEnum(NOSUCHSERVER); addEnum(NOSUCHCHANNEL); addEnum(CANNOTSENDTOCHAN);
          addEnum(TOOMANYCHANNELS); addEnum(WASNOSUCHNICK); addEnum(TOOMANYTARGETS); addEnum(NOSUCHSERVICE);
          addEnum(NOORIGIN); addEnum(NORECIPIENT); addEnum(NOTEXTTOSEND); addEnum(NOTOPLEVEL);
          addEnum(WILDTOPLEVEL); addEnum(BADMASK); addEnum(UNKNOWNCOMMAND); addEnum(NOMOTD);
          addEnum(NOADMININFO); addEnum(FILEERROR); addEnum(NONICKNAMEGIVEN); addEnum(ERRONEUSNICKNAME);
          addEnum(NICKNAMEINUSE); addEnum(NICKCOLLISION); addEnum(UNAVAILRESOURCE); addEnum(USERNOTINCHANNEL);
          addEnum(NOTONCHANNEL); addEnum(USERONCHANNEL); addEnum(NOLOGIN); addEnum(SUMMONDISABLED);
          addEnum(USERSDISABLED); addEnum(NOTREGISTERED); addEnum(NEEDMOREPARAMS); addEnum(ALREADYREGISTERED);
          addEnum(NOPERMFORHOST); addEnum(PASSWDMISMATCH); addEnum(YOUREBANNEDCREEP); addEnum(YOUWILLBEBANNED);
          addEnum(KEYSET); addEnum(CHANNELISFUL); addEnum(UNKNOWNMODE); addEnum(INVITEONLYCHAN);
          addEnum(BANNEDFROMCHAN); addEnum(BADCHANNELKEY); addEnum(BADCHANMASK); addEnum(NOCHANMODES);
          addEnum(BANLISTFULL); addEnum(NOPRIVILEGES); addEnum(CHANOPPRIVSNEEDED); addEnum(CANTKILLSERVER);
          addEnum(RESTRICTED); addEnum(UNIQOPPRIVSNEEDED); addEnum(NOOPERHOST); addEnum(UMODEUNKNOWNFLAG);
          addEnum(USERSDONTMATCH);
        lua_settable(L, -3);
        lua_setglobal(L, "irc");
#undef addEnum

        if (!L) {
          return std::make_tuple(0, "");
        }
        if (luaL_loadfile(L, "script/lua/bootstrap.lua")) {
          return std::make_tuple(1, lua_tostring(L, -1));
        }
        lua_call(L, 0, 0);
        return std::make_tuple(2, "");
      }
    }
  }
}