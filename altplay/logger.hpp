#pragma once
#include <fstream>

namespace altplay
{
  enum LOG_LIST {LOG_NONE = 0, LOG_SCRIPT, LOG_PARSE};
	class logger
	{
	public:
		logger ( std::string path );
		void add_entry ( const std::string& str, LOG_LIST err = LOG_NONE );

	private:
		FILE *file_;
	};
} // end of ns altplay
