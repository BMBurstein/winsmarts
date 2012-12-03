#include "StdAfx.h"
#include "Task.h"


Task::Task(taskProc fn, std::string const &name, int priority, taskProc taskEnd, WinSMARTS* smarts)
	: name(name), priority(priority), origPriority(priority), status(READY)
{
	taskPtr = newTask(fn, NULL, stack + 8192, taskEnd, smarts);
}


Task::~Task(void)
{
}
