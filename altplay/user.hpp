#pragma once
#include <string>

namespace altplay {
class user {
public:
  user(const int level, const std::string& nick);
  ~user() = default;
  user(const user&) = delete;
  user(user&&) = delete;
  user operator=(const user&) = delete;
  user operator=(user&&) = delete;

  int get_access_lvl() const;
  void set_access_lvl(const int lvl);
  int get_login_stamp() const;
  std::string get_nick() const;
  void set_nick(const std::string& nick);
  std::string get_last_quit_host() const;

private:
  int access_level_{0};
  std::string nick_;
  std::string last_quit_host_;
  // login_stamp_ will be updated upon user login and contain the seconds since epoch (time(0));
  int login_stamp_{0};
};
} // end of namespace altplay
