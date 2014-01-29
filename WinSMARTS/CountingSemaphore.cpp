#include "CountingSemaphore.h"
using namespace std;

unsigned long long CountingSemaphore::semIdCounter = 1;

CountingSemaphore::CountingSemaphore(WinSMARTS* SMARTS_, int authorized)
	: SMARTS(SMARTS_),
	  maxAuthorized(authorized),
	  free(authorized),
	  semName("CountingSem_" + to_string(semIdCounter))
{
	semIdCounter++;
}

CountingSemaphore::CountingSemaphore(WinSMARTS* SMARTS_, string name, int authorized)
	: SMARTS(SMARTS_),
	  maxAuthorized(authorized),
	  free(authorized),
	  semName(name)
{
}

void CountingSemaphore::acquire()
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
		SMARTS->log(LOG_LOCK_WAIT, "%u;%s", SMARTS->getCurrentTask(), semName);
		SMARTS->callScheduler(SUSPENDED);
	}

	SMARTS->log(LOG_LOCK_ACQUIRE, "%u;%s", SMARTS->getCurrentTask(), semName);

	if(CS)
		SMARTS->contextSwitchOn();
}
	
void CountingSemaphore::release()
{
	bool CS = SMARTS->getContextSwitchAllow();
	SMARTS->contextSwitchOff();

	SMARTS->log(LOG_LOCK_RELEASE, "%u;%s", SMARTS->getCurrentTask(), semName);

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