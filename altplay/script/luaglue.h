#include "LuaBridge/LuaBridge.h"
#include "message.hpp"
#include <list>
#include <tuple>

#ifndef _LUAGLUE_H_
#define _LUAGLUE_H_
namespace altplay {
	namespace script {
		namespace lua {
			extern lua_State *L;
      std::tuple<int, const char *> init();
      void callhook(std::string, message_struct);
      void callhook(int, message_struct);

			struct irchook
			{
				irchook(lua_State *L) : function(L) {};

				std::string command;
				int icommand;
				luabridge::LuaRef function;
			};
			std::list<irchook> _hooks;
		}
	}
}
#endif // _LUAGLUE_H_