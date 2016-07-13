#include "bot.hpp"
#include "script.h"

namespace altplay {
	namespace script {
		void initScripts() {
			std::unordered_map<std::string, std::string> config{ parser::parse_config("config.conf") };
			if (config.at("lua_module") == "on") {
				int errcode; const char *errinfo;
				std::tie(errcode, errinfo) = lua::init();
					switch (errcode) {
					case 0: printf("unable to create lua state\n"); break;
					case 1: printf("unable to open script.lua: %s\n", errinfo); break;
					case 2: printf("success\n"); break;
				}
			}
		}
	}
}