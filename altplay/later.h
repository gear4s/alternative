#pragma once
#include "LuaBridge/LuaBridge.h"
#include <forward_list>
#include <winsock2.h>

namespace altplay {
  namespace script {
    namespace lua {
      namespace later {
        struct latertoken {
          int lambdaindex;
          bool abs;
          unsigned long long when, delay;
          ~latertoken();
        };
      }
    }
  }
}