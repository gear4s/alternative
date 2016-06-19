#include "logger.hpp"


altplay::logger::logger(std::string path)
{
#ifdef DEBUG_ON
  // can't remember which debug functions
  // are used for files
  // will do research laters
#endif
	this->f = fopen("file.txt", "w");
  if (this->f == NULL) {
      throw std::runtime_error("logger was unable to open the provided file.")
  }
}

// get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
void altplay::logger::add_entry(const std::string& str, LOG_LIST err)
{
  string const LOG_NAME[] = {"NORMAL", "SCRIPT", "CORE"};
	fprintf(this->f, " [ %s %s ] : %s\n", LOG_NAME[err], currentDateTime(), text);
}
