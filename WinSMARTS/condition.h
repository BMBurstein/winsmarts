#include "CountingSemaphore.h"
#include "MutexSemaphore.h"

class Condition
{
	CountingSemaphore *countingBinarySemaphore;
	MutexSemaphore *itsMutexSemaphore;
public:	
	Condition(WinSMARTS* SMARTS_);
	void wait();
	void notify();
	void setItsMutexSemaphore (MutexSemaphore* ms);
};
