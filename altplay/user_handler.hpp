#pragma once
#include <json/json.h>
#include <fstream>

class user_handler
{
public:
	user_handler();
	~user_handler();
private:
	std::fstream user_file_;
};
