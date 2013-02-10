#include "Log.h";

Log::Log()
{
  LogFile.open("WinSMARTSLog.txt", std::ios::out);
}

Log::~Log()
{
  LogFile.close();
  LogFile.open("WinSMARTSLog.txt", std::ios::in);
  LogFile.seekg(0, std::ios::end);
  int length = LogFile.tellg();
  LogFile.close();

  if (length == 0)
  {
    if(remove( "WinSMARTSLog.txt" ) != 0)
	{
      perror("\nError deleting file");
	}
  }
}

void Log::log(const char* line)
{
  LogFile << line;
}

void Log::log(const std::string line)
{
  LogFile << line.c_str();
}