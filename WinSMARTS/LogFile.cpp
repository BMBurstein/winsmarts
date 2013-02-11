#include "LogFile.h"
using namespace std;

LogFile::LogFile(char const* file)
{
  logFile.open(file, ios::out);
}


LogFile::~LogFile(void)
{
  logFile.close();
}

void LogFile::log(const char* msg, size_t len)
{
  logFile << msg;
}