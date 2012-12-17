#include "StdAfx.h"
#include "Task.h"


Task::Task(TaskProc fn, std::string const &name, int priority, TaskProc taskEnd, WinSMARTS* smarts)
	: name(name), priority(priority), origPriority(priority), status(READY)
{
	taskPtr = newTask(fn, NULL, stack + 65536, taskEnd, smarts);
}

Task::Task(TaskProc fn, void* param, std::string const &name, int priority, TaskProc taskEnd, WinSMARTS* smarts)
	: name(name), priority(priority), origPriority(priority), status(READY)
{
	taskPtr = newTask(fn, param, stack + 65536, taskEnd, smarts);
}

void Task::sleepDecr()
{
	if(status == SLEEPING)
	{
		if(sleepCounter > 0)
			--sleepCounter;
		if(sleepCounter == 0)
			status = READY;
	}
}
