#include "LuaBridge/LuaBridge.h"
#include <forward_list>

#ifdef __WIN32
#include <winsock2.h>
#endif

#ifdef __GNUG__
#include <cmath>
#include <cxxabi.h>
#endif

#ifndef _LATER_H_
#define _LATER_H_
namespace altplay {
  namespace script {
    template<typename T>
    std::string classname() {
      /// Demangle strings using the compiler-provided demangle function.
      const char* mangled = typeid(T).name();
#ifdef __GNUG__
      char* demangled;
      int status;
      demangled = abi::__cxa_demangle(mangled, 0, 0, &status);
      std::string ret = demangled ? demangled : mangled;
      free(demangled);
      return ret;
#else
      return mangled;
#endif
    }
    template<typename T> std::string classname(T&) { return classname<T>(); }

    namespace lua {
      extern lua_State *L;
      namespace later {
        struct latertoken {
          int lambdaindex;
          unsigned long long when, delay;
          ~latertoken();
        };
        latertoken* newlater(lua_State* L);
        void cancel(latertoken& t);
        void check();
        extern unsigned int servermillis;
      }
    }
  }
}
#endif // _LATER_H_