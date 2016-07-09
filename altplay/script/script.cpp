#include "script.h"

namespace altplay {
	namespace script {
		void initScripts() {
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