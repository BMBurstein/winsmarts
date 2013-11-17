#include "CountingSemaphore.h"

CountingSemaphore::CountingSemaphore(WinSMARTS* SMARTS_,int authorized)
	: SMARTS(SMARTS_),
	  maxAuthorized(authorized),
	  free(authorized)
{
}

void CountingSemaphore::acquire()
{
	bool CS = SMARTS->getContextSwitchAllow();
	SMARTS->contextSwitchOff();

	if (free > 0)
	{
		free--;
		SMARTS->contextSwitchOn();
	}
	else
	{
		waitingList.push(SMARTS->getCurrentTask());
		if(CS)
			SMARTS->contextSwitchOn();
		SMARTS->setTaskStatus(SUSPENDED);
		SMARTS->callScheduler();
	}
}
	
void CountingSemaphore::release()
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