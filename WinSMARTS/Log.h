#ifndef LOG_H
#define LOG_H

#include <fstream>

class Log
{
  std::fstream LogFile;

public:
  Log();
  ~Log();
  void log(const char* line);
  void log(const std::string line);
};

#endif // LOG_H