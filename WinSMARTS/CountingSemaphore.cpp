#include "CountingSemaphore.h"
using namespace std;

unsigned long long CountingSemaphore::semIdCounter = 1;

CountingSemaphore::CountingSemaphore(WinSMARTS* SMARTS_, int authorized)
	: SMARTS(SMARTS_),
	  maxAuthorized(authorized),
	  free(authorized),
	  name("CountingSem_" + to_string(semIdCounter++))
{
	SMARTS->log(LOG_LOCK_COUNT, "%d;%s", free, name.c_str());
}

CountingSemaphore::CountingSemaphore(WinSMARTS* SMARTS_, string name, int authorized)
	: SMARTS(SMARTS_),
	  maxAuthorized(authorized),
	  free(authorized),
	  name(name)
{
	SMARTS->log(LOG_LOCK_COUNT, "%d;%s", free, name.c_str());
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
		SMARTS->log(LOG_LOCK_WAIT, "%u;%s", SMARTS->getCurrentTask(), name.c_str());
		SMARTS->callScheduler(SUSPENDED);
	}

	SMARTS->log(LOG_LOCK_ACQUIRE, "%u;%d;%s", NO_TASK, free, name.c_str());

	if(CS)
		SMARTS->contextSwitchOn();
}
	
void CountingSemaphore::release()
{
	bool CS = SMARTS->getContextSwitchAllow();
	SMARTS->contextSwitchOff();

	if(!waitingList.empty())
	{
		SMARTS->setTaskStatus(waitingList.front(), READY);
		SMARTS->log(LOG_LOCK_RELEASE, "%u;%d;%s", waitingList.front(), free, name.c_str());
		waitingList.pop();
	}
	else
	{
		if (free + 1 <= maxAuthorized)
			free++;
		SMARTS->log(LOG_LOCK_RELEASE, "%u;%d;%s", NO_TASK, free, name.c_str());
	}

	if(CS)
		SMARTS->contextSwitchOn();
}