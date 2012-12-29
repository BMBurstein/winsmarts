#include "StdAfx.h"
#include "WinSMARTS.h"


int RR::schedule() const
{
	int nextTask = smarts->getCurrentTask() + 1;
	if(nextTask == smarts->getTotalTasks())
		nextTask = 1;

	//passes on all the tasks array and checks if there is ready task 
	for(int count=1; smarts->getStatus(nextTask) != READY && count<smarts->getTotalTasks(); ++count)
	{
		if(++nextTask == smarts->getTotalTasks())
			nextTask = 1;
	}

	//if not found any ready task, go back to "System Idle" task
	if(smarts->getStatus(nextTask) != READY)
		nextTask = 0;
	return nextTask;
}