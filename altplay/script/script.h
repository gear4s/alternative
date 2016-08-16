#ifndef _SCRIPT_H_
#define _SCRIPT_H_                                         
#include <string>
#include <cstdarg>    // va_start, va_end, std::va_list
#include <vector>
#include "luaglue.h"

#ifdef WIN32
#include <shlobj.h>
#else
#include <dirent.h>
#define _vsnprintf vsnprintf
#endif

namespace altplay {
	namespace script {
		void initScripts();
		const char *strformat(std::string str, va_list args);
		const char *strformat(std::string str, ...);
		
		bool listdir(const char *dirname, bool rel, const char *ext, std::vector<char *> &files);
	}
}
#endif // _SCRIPT_H_