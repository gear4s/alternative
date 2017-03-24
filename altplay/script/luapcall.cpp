#include <lua.hpp>
#include "luapcall.h"

#define ERROR_HANDLER_KEY "pcall_error_handler"
namespace altplay {
	namespace script {
		namespace lua {
      static int on_error(lua_State * L) {
        if(lua_type(L, 1) != LUA_TTABLE) {
          lua_getglobal(L, "debug");
          if(lua_type(L, -1) != LUA_TTABLE) {
            lua_pop(L, 1);
            return 1;
          }
          
          lua_getfield(L, -1, "traceback");
          if(lua_type(L, -1) != LUA_TFUNCTION) {
            lua_pop(L, 1);
            return 1;
          }
          
          lua_pushvalue(L, 1);
          lua_pushinteger(L, 2);
          lua_pcall(L, 2, 1, 0);
          
          lua_newtable(L);
          lua_pushinteger(L, 1);
          lua_pushvalue(L, -3);
          lua_settable(L, -3);
          
          return 1;
        }
        
        lua_pushvalue(L, 1);
        return 1;
      }
      
      void get_error_handler(lua_State * L) {
        lua_pushcfunction(L, on_error);
      }
    }
  }
}
