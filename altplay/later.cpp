#include "later.h"

namespace altplay {
  namespace script {
    namespace lua {
      namespace later {
        int stackdumperref = LUA_NOREF;

        template<typename F, typename Err>
        void lua_cppcall(const F& f, const Err& err) {
          lua_rawgeti(L, LUA_REGISTRYINDEX, stackdumperref);
          int dumperpos = lua_gettop(L);
          lua_pushcfunction(L, [](lua_State* L) {
            try { (*(const F*)lua_touserdata(L, 1))(); return 0; }
            catch (const std::exception& e) {
              lua_pushfstring(L, "exception %s: %s", classname(e).c_str(), e.what());
            }
            catch (...) {
#if defined(LUA_USE_LONGJMP) || defined(LUA_USE_ULONGJMP)
              lua_pushstring(L, "C++ exception (not a std::exception)");
#else
              throw;
#endif
            }
            return lua_error(L);
          });
          lua_pushlightuserdata(L, const_cast<F*>(&f));
          int result = lua_pcall(L, 1, 0, dumperpos);
          lua_remove(L, dumperpos);
          if (result == LUA_OK) return;
          std::string e = lua_tostring(L, -1);
          lua_pop(L, 1);
          err(e);
        }

        unsigned int servermillis, curtime;
        unsigned int timeBase;
        auto time_get(void) -> unsigned int
        {
          return (unsigned int)timeGetTime() - timeBase;
        }

        void time_set(unsigned int newTimeBase)
        {
          timeBase = (unsigned int)timeGetTime() - newTimeBase;
        }

        latertoken::~latertoken() {
          if (lambdaindex != LUA_NOREF) luaL_unref(lua::L, LUA_REGISTRYINDEX, lambdaindex);
        }

        std::forward_list<latertoken*> serv;
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
        auto newlater(lua_State* L) -> latertoken* {
          auto fdelay = luaL_checknumber(L, 1);
          luaL_argcheck(L, fdelay > 0, 1, "invalid delay");
          if (std::isinf(fdelay)) return &neverhappening;
          unsigned long long delay = std::ceil(fdelay);
          lua_pushvalue(L, 2);
          int lambdaindex = luaL_ref(L, LUA_REGISTRYINDEX);
          latertoken* l = 0;
          try {
            insert(l = new latertoken{ lambdaindex, delay + (servermillis), delay * (unsigned long long)lua_toboolean(L, 3) }, serv);
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
          if (currentlambda) currentlambda->delay = 0;
        }

        void cancel(latertoken& t) {
          if (&t == &neverhappening) return;
          if (&t == currentlambda) {
            currentlambda->delay = 0;
            return;
          }
          auto& list = serv;
          for (auto it = list.before_begin(), prev = (it++, list.before_begin()); it != list.end(); it++, prev++) if (&t == *it) {
            list.erase_after(prev);
            delete &t;
            break;
          }
        }

        void check(unsigned long long now, std::forward_list<latertoken*>& list) {
          while (!list.empty() && list.front()->when <= now) {
            auto l = currentlambda = list.front();
            list.pop_front();
            lua_cppcall([l, &list] {
              lua_rawgeti(L, LUA_REGISTRYINDEX, l->lambdaindex);
              lua_call(L, 0, 0);
              if (!l->delay) {
                delete l;
                return;
              }
              l->when += l->delay;
              insert(l, list);
            }, [l, &list](std::string& err) {
              printf("One later resulted in an error%s: %s", l->delay ? " and has been cancelled" : "", err.c_str());
              delete l;
            });
            currentlambda = 0;
          }
        }

        void check() {
          check(servermillis, serv);
        }

        void fini() {
          for (auto& laterlist : { later::serv }) for (auto l : laterlist) delete l;
          later::serv.clear();
        }
      }
    }
  }
}