#include "LogFile.h"
using namespace std;

LogFile::LogFile(char const* file) : fileName(file)
{
	logFile.open(file, ios::out | ios::trunc);
}

LogFile::~LogFile(void)
{
	logFile.close();
}

void LogFile::log(LogMsg evt, const char* msg, size_t len)
{
	logFile << msg << '\n';
}

void LogFile::clear()
{
	logFile.close();
	logFile.open(fileName, ios::out | ios::trunc);
}