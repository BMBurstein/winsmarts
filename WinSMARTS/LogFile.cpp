#include "LogFile.h"
#include <iomanip>
using namespace std;

static const char* LogMsgName(LogMsg m)
{
	static char buff[15];

	switch(m)
	{
		case LOG_START:					return "LOG_START";
		case LOG_END:					return "LOG_END";
		case LOG_NEW_TASK:				return "LOG_NEW_TASK";
		case LOG_CONTEXT_SWITCH:		return "LOG_CONTEXT_SWITCH";
		case LOG_CONTEXT_SWITCH_ON:		return "LOG_CONTEXT_SWITCH_ON";
		case LOG_CONTEXT_SWITCH_OFF:	return "LOG_CONTEXT_SWITCH_OFF";
		case LOG_TIMER:					return "LOG_TIMER";
		case LOG_DEADLOCK:				return "LOG_DEADLOCK";
		case LOG_TIME_OUT:				return "LOG_TIME_OUT";
		case LOG_REDECLARE:				return "LOG_REDECLARE";
		case LOG_TASK_STATUS_CHANGE:	return "LOG_TASK_STATUS_CHANGE";
		case LOG_TASK_PROP_SET:			return "LOG_TASK_PROP_SET";
		case LOG_LOCK_ACQUIRE:			return "LOG_LOCK_ACQUIRE";
		case LOG_LOCK_RELEASE:			return "LOG_LOCK_RELEASE";
		case LOG_LOCK_WAIT:				return "LOG_LOCK_WAIT";
		case LOG_LOCK_COUNT:			return "LOG_LOCK_COUNT";
		default:						return itoa(m, buff, 10);
	}
}

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
	logFile << setw(25) << left << LogMsgName(evt) << setw(0) << ": " << msg << '\n';
}

void LogFile::clear()
{
	logFile.close();
	logFile.open(fileName, ios::out | ios::trunc);
}