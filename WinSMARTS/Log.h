#pragma once
#ifndef LOG_H
#define LOG_H

//! Message codes for logging
enum LogMsg
{
	// WinSMARTS messages
	LOG_START,
	LOG_END,
	LOG_NEW_TASK,
	LOG_CONTEXT_SWITCH,
	LOG_CONTEXT_SWITCH_ON,
	LOG_CONTEXT_SWITCH_OFF,
	LOG_TIMER,
	LOG_DEADLOCK,
	LOG_TIME_OUT,
	LOG_REDECLARE,

	// Task messages
	LOG_TASK_STATUS_CHANGE,

	// Locks meassages
	LOG_LOCK_ACQUIRE,
	LOG_LOCK_RELEASE,
	LOG_LOCK_WAIT,
	LOG_LOCK_COUNT,
};

class Log
{
public:
	virtual ~Log() {}

	virtual void log(LogMsg evt, const char* msg, size_t len) {}
	virtual void clear() {}
};

#endif // LOG_H