#include "user.hpp"

altplay::user::user(const int level, const std::string &nick) : access_level_{level}, nick_{nick} {

}
int altplay::user::get_access_lvl() const {
  return access_level_;
}
int altplay::user::get_login_stamp() const {
  return login_stamp_;
}
std::string altplay::user::get_nick() const {
  return nick_;
}
std::string altplay::user::get_last_quit_host() const {
  return last_quit_host_;
}
void altplay::user::set_access_lvl(const int lvl) {
  access_level_ = lvl;
}
void altplay::user::set_nick(const std::string &nick) {
  nick_ = nick;
}
