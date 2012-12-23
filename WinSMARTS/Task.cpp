#include "StdAfx.h"
#include "Task.h"


Task::Task(TaskProc fn, std::string const &namep, int priorityp, TaskProc taskEnd, WinSMARTS* smarts)
	: name(namep), priority(priorityp), origPriority(priorityp), status(READY)
{
	taskPtr = newTask(fn, smarts, stack + 65536, taskEnd, smarts);		//??
}

//decreases sleep counter when passes on asleep task
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
