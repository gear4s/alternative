#include <cstdint>
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

			int init() {
				L = luaL_newstate();
				luaL_openlibs(L);

				getGlobalNamespace(L)
					.beginClass<message_struct>("message_struct")
					.addData("nick", &message_struct::nick)
					.endClass()

					.beginNamespace("bot")
						.addFunction("hook", &hook)
					.endNamespace();

				if (!L) {
					return 0;
				}
				if (luaL_loadfile(L, "script.lua")){
					return 1;
				}
				lua_call(L, 0, 0);
				return 2;
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