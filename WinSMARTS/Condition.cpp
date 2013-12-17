#include "Condition.h"

Condition::Condition(WinSMARTS* SMARTS_)
{
	countingBinarySemaphore = new CountingSemaphore(SMARTS_);
}

void Condition::wait()
{
	itsMutexSemaphore->release(); 		// release monintor
	countingBinarySemaphore->acquire();	// acquire condition
	itsMutexSemaphore->acquire();		// acquire monintor
}	
void Condition::notify()
{
	countingBinarySemaphore->release();
}

void Condition::setItsMutexSemaphore (MutexSemaphore* mutexSemaphore)
{
	itsMutexSemaphore = mutexSemaphore;
}
