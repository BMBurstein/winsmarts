#include "MutexSemaphore.h"
using namespace std;

unsigned long long MutexSemaphore::semIdCounter = 1;

MutexSemaphore::MutexSemaphore(WinSMARTS* SMARTS_)
	: SMARTS(SMARTS_),
	  owner(-1),
	  level(0),
	  isFree(true),
	  name("MutexSem_" + to_string(semIdCounter++))
{
	SMARTS->log(LOG_LOCK_COUNT, "%d;%s", level, name.c_str());
}

MutexSemaphore::MutexSemaphore(WinSMARTS* SMARTS_, string name)
	: SMARTS(SMARTS_),
	  owner(-1),
	  level(0),
	  isFree(true),
	  name(name)
{
	SMARTS->log(LOG_LOCK_COUNT, "%d;%s", level, name.c_str());
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
		SMARTS->log(LOG_LOCK_WAIT, "%u;%s", SMARTS->getCurrentTask(), name.c_str());
		SMARTS->callScheduler(SUSPENDED);
	}

	owner = SMARTS->getCurrentTask();
	level++;

	SMARTS->log(LOG_LOCK_ACQUIRE, "%u;%d;%s", SMARTS->getCurrentTask(), level, name.c_str());

	if(CS)
		SMARTS->contextSwitchOn();
}

void MutexSemaphore::release()
{
	bool CS = SMARTS->getContextSwitchAllow();
	SMARTS->contextSwitchOff();


	if (owner == SMARTS->getCurrentTask())
	{
		if(--level)
		{
			SMARTS->log(LOG_LOCK_COUNT, "%d;%s", level, name.c_str());
			return;
		}
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
			SMARTS->log(LOG_LOCK_RELEASE, "%u;%d;%s", SMARTS->getCurrentTask(), level, name.c_str());
		}
	}

	if(CS)
		SMARTS->contextSwitchOn();
}