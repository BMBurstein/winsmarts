#include "MutexSemaphore.h"
#include "Condition.h"

class Monitor
{
protected:
	int size;
	MutexSemaphore *mutexSemaphore;
	Condition **conditions;
public:
	Monitor(int size, WinSMARTS* SMARTS_);
	void entryM();
	void exitM();
	void WaitCondition(int index);
	void NotifyCondition(int index);
};
