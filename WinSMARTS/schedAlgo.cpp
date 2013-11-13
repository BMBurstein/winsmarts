#include "WinSMARTS.h"


size_t RoundRobin(WinSMARTS* SMARTS)
{
	if (SMARTS->getTotalTasks() == 1)
		return 0;

	tid_t nextTask = SMARTS->getCurrentTask() + 1;
	if(nextTask == SMARTS->getTotalTasks())
		nextTask = 1;

	//passes on all the tasks array and checks if there is ready task 
	for(size_t count=1; SMARTS->getTaskStatus(nextTask) != READY && count < SMARTS->getTotalTasks(); ++count)
	{
		if(++nextTask == SMARTS->getTotalTasks())
			nextTask = 1;
	}

	//if not found any ready task, go back to "System Idle" task
	if(SMARTS->getTaskStatus(nextTask) != READY)
		nextTask = 0;
	return nextTask;
}