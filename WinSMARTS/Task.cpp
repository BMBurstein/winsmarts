#include "Task.h"
#include "WinSMARTS.h"
#include <cstdint>


Task::Task(TaskProc fn, size_t id_, std::string const &name_, int priority_, TaskProc taskEnd, WinSMARTS* SMARTS_)
	: id             (id_),
	name          (name_),
	priority      (priority_),
	origPriority  (priority_),
	status        (READY),
	expectedEvent (NULL),
	CSOff         (false),
	SMARTS        (SMARTS_)
{
	taskPtr = newTask(fn, SMARTS_, (char*)((uintptr_t)(stack + 65536 + STACK_ALIGN - 1) & ~(STACK_ALIGN - 1)), taskEnd, SMARTS_);    //initialize the stack of the new task and save stack pointer
}

void Task::sleepDecr()
{
	if(status == SLEEPING)
	{
		if(sleepCounter > 0)
			--sleepCounter;
		if(sleepCounter == 0)
			setStatus(READY);
	}
}

char* Task::taskStatusToString(taskStatus stat)
{
	switch (stat)
	{
	case READY:
		return "READY";
	case NOT_ACTIVE:
		return "NOT_ACTIVE";
	case SUSPENDED:
		return "SUSPENDED";
	case SLEEPING:
		return "SLEEPING";

	default:
		return "Status not recognized..";
	}
}

void Task::setStatus(taskStatus stat)
{
	status = stat;

	SMARTS->log(LOG_TASK_STATUS_CHANGE, "%d;%s", id, taskStatusToString(stat));
}
