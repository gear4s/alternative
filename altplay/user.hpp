#pragma once
#include <string>

namespace altplay
{
	class user
	{
	private:
		int access_level_{0};
		std::string nick_;
		std::string last_quit_host_;
		// login_stamp_ will be updated upon user login and contain the seconds since epoch (time(0));
		int login_stamp_{0};
	};
}
