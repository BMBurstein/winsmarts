#include "MutexSemaphore.h"
using namespace std;

unsigned long long MutexSemaphore::semIdCounter = 1;

MutexSemaphore::MutexSemaphore(WinSMARTS* SMARTS_)
	: SMARTS(SMARTS_),
	  owner(-1),
	  level(0),
	  isFree(true),
	  name("MutexSem_" + to_string(semIdCounter))
{
}

MutexSemaphore::MutexSemaphore(WinSMARTS* SMARTS_, string name)
	: SMARTS(SMARTS_),
	  owner(-1),
	  level(0),
	  isFree(true),
	  name(name)
{
}

void MutexSemaphore::acquire()
{
	bool CS = SMARTS->getContextSwitchAllow();
	SMARTS->contextSwitchOff();

	if (isFree || owner == SMARTS->getCurrentTask())
		isFree = false;
	else
	{
		waitingList.push(SMARTS->getCurrentTask());
		SMARTS->log(LOG_LOCK_WAIT, "%u;%s", SMARTS->getCurrentTask(), name);
		SMARTS->callScheduler(SUSPENDED);
	}

	owner = SMARTS->getCurrentTask();
	level++;

	SMARTS->log(LOG_LOCK_ACQUIRE, "%u;%s", SMARTS->getCurrentTask(), name);

	if(CS)
		SMARTS->contextSwitchOn();
}

void MutexSemaphore::release()
{
	bool CS = SMARTS->getContextSwitchAllow();
	SMARTS->contextSwitchOff();

	SMARTS->log(LOG_LOCK_RELEASE, "%u;%s", SMARTS->getCurrentTask(), name);

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