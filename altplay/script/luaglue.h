#pragma once
#include "LuaBridge/LuaBridge.h"
#include "message.hpp"
#include <list>

namespace altplay {
	namespace script {
		namespace lua {
			extern lua_State *L;
			extern int init();
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