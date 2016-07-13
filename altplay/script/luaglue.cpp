#include <cstdint>
#include <csignal>
#ifndef _WIN32
#include <sys/resource.h>
#include <sys/prctl.h>
#endif
#include "script.h"

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

      auto init() -> std::tuple<int, const char *> {
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
          .endNamespace()
          .beginNamespace("irc")
            .beginNamespace("reply")
            .endNamespace()
            .beginNamespace("error")
            .endNamespace()
          .endNamespace();
#define addEnum(n)    lua_pushstring(L, #n); lua_pushnumber(L, n); lua_rawset(L, -3)
        lua_getglobal(L, "irc");
        lua_getfield(L, -1, "reply");
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
        lua_getfield(L, -2, "error");
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
#undef addEnum

        if (!L) {
          return std::make_tuple(0, "");
        }
        if (luaL_loadfile(L, "script.lua")){
          return std::make_tuple(1, lua_tostring(L, -1));
        }
        lua_call(L, 0, 0);
        return std::make_tuple(2, "");
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
    }
  }
}