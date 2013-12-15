#include "countingSemaphore.h"

unsigned int countingSemaphore::semIdCounter = 1;

countingSemaphore::countingSemaphore(WinSMARTS* SMARTS_,int authorized)
	: SMARTS(SMARTS_),
	  maxAuthorized(authorized),
	  free(authorized),
	  semId(semIdCounter)
{
	semIdCounter++;
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
		SMARTS->setTaskProp(COUNTING_SEM_WAIT, semId);
		SMARTS->callScheduler(SUSPENDED);
		SMARTS->setTaskProp(COUNTING_SEM_WAIT, PROP_NO_VAL);
	}

	SMARTS->setTaskProp(COUNTING_SEM_ACQ, semId);

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

	SMARTS->setTaskProp(COUNTING_SEM_ACQ, PROP_NO_VAL);

	if(CS)
		SMARTS->contextSwitchOn();
}