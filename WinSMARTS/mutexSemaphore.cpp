#include "mutexSemaphore.h"

mutexSemaphore::mutexSemaphore(WinSMARTS* SMARTS_)
	: SMARTS(SMARTS_),
	  owner(-1),
	  level(0),
	  isFree(true)
{
}

void mutexSemaphore::acquire()
{
	bool CS = SMARTS->getContextSwitchAllow();
	SMARTS->contextSwitchOff();

	if (isFree || owner == SMARTS->getCurrentTask())
		isFree = false;
	else
	{
		waitingList.push(SMARTS->getCurrentTask());
		SMARTS->callScheduler(SUSPENDED);
	}

	owner = SMARTS->getCurrentTask();
	level++;

	if(CS)
		SMARTS->contextSwitchOn();
}

void mutexSemaphore::release()
{
	bool CS = SMARTS->getContextSwitchAllow();
	SMARTS->contextSwitchOff();

	if (owner == SMARTS->getCurrentTask())
	{
		if(--level)
			return;
		else
		{
			owner = -1;
			if(!waitingList.empty())
			{
				SMARTS->setTaskStatus(waitingList.front(), READY);
				waitingList.pop();
			}
			else
				isFree = true;
		}
	}

	if(CS)
		SMARTS->contextSwitchOn();
}