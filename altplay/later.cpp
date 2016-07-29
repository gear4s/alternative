
#include "later.h"

namespace altplay {
  namespace script {
    namespace lua {
      namespace later {
        unsigned int servermillis, curtime;
        unsigned int timeBase;
        auto enet_time_get(void) -> unsigned int
        {
          return (unsigned int)timeGetTime() - timeBase;
        }

        void enet_time_set(unsigned int newTimeBase)
        {
          timeBase = (unsigned int)timeGetTime() - newTimeBase;
        }

        namespace lua {
          extern lua_State *L;
        }

        latertoken::~latertoken() {
          if (lambdaindex != LUA_NOREF) luaL_unref(lua::L, LUA_REGISTRYINDEX, lambdaindex);
        }

        std::forward_list<latertoken*> abs, serv;
        unsigned long long longtotalmillis = 0;
        latertoken* currentlambda = 0;

        void insert(latertoken* t, std::forward_list<latertoken*>& list) {
          auto it = list.before_begin(), prev = (it++, list.before_begin());
          for (; it != list.end(); it++, prev++) if (t->when < (*it)->when) {
            list.insert_after(prev, t);
            return;
          }
          list.insert_after(prev, t);
        }

        static latertoken neverhappening{ LUA_NOREF };
        auto newlater(lua_State* L, bool abs) -> latertoken* {
          auto fdelay = luaL_checknumber(L, 1);
          luaL_argcheck(L, fdelay > 0, 1, "invalid delay");
          if (std::isinf(fdelay)) return &neverhappening;
          unsigned long long delay = std::ceil(fdelay);
          lua_pushvalue(L, 2);
          int lambdaindex = luaL_ref(L, LUA_REGISTRYINDEX);
          latertoken* l = 0;
          try {
            insert(l = new latertoken{ lambdaindex, abs, delay + (abs ? longtotalmillis : servermillis), delay * (unsigned long long)lua_toboolean(L, 3) }, abs ? later::abs : game);
            return l;
          }
          catch (...) {
            if (l) delete l; else luaL_unref(L, LUA_REGISTRYINDEX, lambdaindex);
          }
          luaL_error(L, "OOM when adding a new later");
          return 0;
        }

        void clear() {
          for (auto t : serv) delete t;
          serv.clear();
          if (currentlambda && !currentlambda->abs) currentlambda->delay = 0;
        }

        void cancel(latertoken& t) {
          if (&t == &neverhappening) return;
          if (&t == currentlambda) {
            currentlambda->delay = 0;
            return;
          }
          auto& list = t.abs ? abs : serv;
          for (auto it = list.before_begin(), prev = (it++, list.before_begin()); it != list.end(); it++, prev++) if (&t == *it) {
            list.erase_after(prev);
            delete &t;
            break;
          }
        }
      }
    }
  }
}