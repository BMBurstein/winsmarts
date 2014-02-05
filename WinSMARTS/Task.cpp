#include "Task.h"
#include "WinSMARTS.h"
#include <cstdint>


Task::Task(TaskProc fn, size_t id_, std::string const &name_, int priority_, TaskProc taskEnd, WinSMARTS* SMARTS_, size_t stackSize)
	: id            (id_),
	  name          (name_),
	  priority      (priority_),
	  origPriority  (priority_),
	  status        (READY),
	  expectedEvent (NULL),
	  CSOff         (false),
	  SMARTS        (SMARTS_)
{
	stack = new char[stackSize + STACK_ALIGN - 1];
	taskPtr = newTask(fn, SMARTS_, (char*)((uintptr_t)(stack + stackSize + STACK_ALIGN - 1) & ~(STACK_ALIGN - 1)), taskEnd, SMARTS_);    //initialize the stack of the new task and save stack pointer
}

Task::~Task()
{
	delete[] stack;
}

void Task::sleepDecr()
{
	if(status == SLEEPING)
	{
		if(sleepCounter > 0)
			--sleepCounter;
		if(sleepCounter == 0)
			SMARTS->setTaskStatus(id, READY);
	}
}

void Task::setStatus(taskStatus stat)
{
	status = stat;

	SMARTS->log(LOG_TASK_STATUS_CHANGE, "%d;%d", id, stat);
}