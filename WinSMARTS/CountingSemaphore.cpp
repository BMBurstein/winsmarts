#include "countingSemaphore.h"

countingSemaphore::countingSemaphore(WinSMARTS* SMARTS_,int authorized)
	: SMARTS(SMARTS_),
	  maxAuthorized(authorized),
	  free(authorized)
{
}

void countingSemaphore::acquire()
{
	bool CS = SMARTS->getContextSwitchAllow();
	SMARTS->contextSwitchOff();

	if (free > 0)
	{
		free--;
	}
	else
	{
		waitingList.push(SMARTS->getCurrentTask());
		SMARTS->setTaskStatus(SUSPENDED);
		SMARTS->callScheduler();
	}

	if(CS)
		SMARTS->contextSwitchOn();
}
	
void countingSemaphore::release()
{
	bool CS = SMARTS->getContextSwitchAllow();
	SMARTS->contextSwitchOff();

	if(!waitingList.empty())
	{
		SMARTS->setTaskStatus(waitingList.front(), READY);
		waitingList.pop();
	}
	else
	{
		if (free + 1 <= maxAuthorized)
			free++;
	}

	if(CS)
		SMARTS->contextSwitchOn();
}