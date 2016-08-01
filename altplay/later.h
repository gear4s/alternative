#pragma once
#include "LuaBridge/LuaBridge.h"
#include <forward_list>
#include <winsock2.h>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

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