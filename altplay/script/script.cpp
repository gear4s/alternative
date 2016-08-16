#include "bot.hpp"
#include "script.h"

namespace altplay {
	namespace script {
		const char *strformat(std::string str, va_list args) {
			static char buf[512];
			_vsnprintf(buf, sizeof(buf), str.c_str(), args);
			buf[512 - 1] = 0;
			return buf;
		}
		const char *strformat(std::string str, ...) {
			va_list args;
			va_start(args, str);
			const char *r = strformat(str, args);
			va_end(args);
			return r;
		}
		
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
		
		bool listdir(const char *dirname, bool rel, const char *ext, std::vector<char *> &files) {
			size_t extsize = ext ? strlen(ext)+1 : 0;
#ifdef WIN32
			const char *pathname = strformat(rel ? ".\\%s\\*.%s" : "%s\\*.%s", dirname, ext ? ext : "*");
			WIN32_FIND_DATA FindFileData;
			HANDLE Find = FindFirstFile(pathname, &FindFileData);
			if(Find != INVALID_HANDLE_VALUE) {
				do {
					if(!ext)
						files.push_back(FindFileData.cFileName);
					else {
						size_t namelen = strlen(FindFileData.cFileName); 
						if(namelen > extsize)  { 
							namelen -= extsize;
							if(FindFileData.cFileName[namelen] == '.' && strncmp(FindFileData.cFileName+namelen+1, ext, extsize-1)==0)
								files.push_back(FindFileData.cFileName);
						}
					}
				} while(FindNextFile(Find, &FindFileData));
				FindClose(Find);
				return true;
			}
#else
			const char *pathname = strformat(rel ? "./%s" : "%s", dirname);
			DIR *d = opendir(pathname);
			if(d) {
				struct dirent *de;
				while((de = readdir(d)) != NULL) {
					if(!ext)
						files.push_back(de->d_name);
					else {
						size_t namelen = strlen(de->d_name);
						if(namelen > extsize) {
							namelen -= extsize;
							if(de->d_name[namelen] == '.' && strncmp(de->d_name+namelen+1, ext, extsize-1)==0)
								files.push_back(de->d_name);
						}
					}
				}
				closedir(d);
				return true;
			}
#endif
			else return false;
		}
	}
}