#pragma once
#include <string>

namespace altplay
{
	struct message_struct
	{
		std::string nick{"null"}, hostmask{"null"}, ident{"null"}, prefix{"null"}, params{"null"}, command{"null"},
			target{"null"}, message{"null"};
		bool is_server_message{false};
	};
} // end of ns altplay
