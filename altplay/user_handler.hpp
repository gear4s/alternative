#pragma once
#include <fstream>
#include <list>
#include "user.hpp"

namespace altplay
{
	class user_handler
	{
	public:
		explicit user_handler ( );
	private:
		std::list<user> user_list_;
		std::fstream user_file_;
	};
}
