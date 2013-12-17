#include "Monitor.h"

Monitor::Monitor(int size, WinSMARTS* SMARTS_):size(size)
{
	mutexSemaphore = new MutexSemaphore(SMARTS_);
	conditions =  new Condition* [size];
	for (int i=0; i<size; i++)
	{
		conditions[i] = new Condition(SMARTS_);
		conditions[i]->setItsMutexSemaphore(mutexSemaphore);
	}
}

void Monitor::entryM()
{
	mutexSemaphore->acquire();
}
	
void Monitor::exitM()
{
	mutexSemaphore->release();
}

void Monitor::WaitCondition(int index)
{
	// ### carefull!! id received and index used!!!!!!!
	conditions[index]->wait();
}

void Monitor::NotifyCondition(int index)
{
	conditions[index]->notify();
}
