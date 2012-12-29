#include "StdAfx.h"
#include "WinSMARTS.h"


int RoundRobin(WinSMARTS* SMARTS)
{
	int nextTask = SMARTS->getCurrentTask() + 1;
	if(nextTask == SMARTS->getTotalTasks())
		nextTask = 1;

	//passes on all the tasks array and checks if there is ready task 
	for(int count=1; SMARTS->getStatus(nextTask) != READY && count<SMARTS->getTotalTasks(); ++count)
	{
		if(++nextTask == SMARTS->getTotalTasks())
			nextTask = 1;
	}

	//if not found any ready task, go back to "System Idle" task
	if(SMARTS->getStatus(nextTask) != READY)
		nextTask = 0;
	return nextTask;
}