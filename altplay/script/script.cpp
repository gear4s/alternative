#include "script.h"

namespace altplay {
	namespace script {
		void initScripts() {
			lua::init();
			// errors below

			switch (lua::init()) {
			case 0: printf("unable to create lua state\n"); break;
			case 1: printf("unable to open script.lua\n"); break;
			case 2: printf("success\n"); break;
			}
		}
	}
}