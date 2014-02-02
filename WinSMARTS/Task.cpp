#include "Task.h"
#include "WinSMARTS.h"
#include <cstdint>


Task::Task(TaskProc fn, size_t id_, std::string const &name_, int priority_, TaskProc taskEnd, WinSMARTS* SMARTS_, unsigned int cyclePeriod_, unsigned int cyclesCount_, size_t stackSize)
	: id            (id_),
	  name          (name_),
	  priority      (priority_),
	  origPriority  (priority_),
	  status        (READY),
	  expectedEvent (NULL),
	  CSOff         (false),
	  SMARTS        (SMARTS_),
	  cyclePeriod	(cyclePeriod_),
	  leftCyclePeriod(cyclePeriod_),
	  cyclesCount	(cyclesCount_)
{
	stack = new char[stackSize + STACK_ALIGN - 1];
	taskPtr = newTask(fn, SMARTS_, (char*)((uintptr_t)(stack + stackSize + STACK_ALIGN - 1) & ~(STACK_ALIGN - 1)), taskEnd, SMARTS_);    //initialize the stack of the new task and save stack pointer

	stackBackupSize = (unsigned int)((char*)((uintptr_t)(stack + stackSize + STACK_ALIGN - 1) & ~(STACK_ALIGN - 1)) - (char *) taskPtr);
	stackBackupSource = (char*)taskPtr;
	stackBackupDest = new char[stackBackupSize];
	for (unsigned int i=0; i<stackBackupSize; i++)
	{
		*(stackBackupDest + i) = *(stackBackupSource + i);
	}
}

Task::~Task()
{
	delete[] stack;
}

void Task::sleepDecr()
{
	if(status == SLEEPING)
	{
		if(sleepCounter > 0)
			--sleepCounter;
		if(sleepCounter == 0)
			SMARTS->setTaskStatus(id, READY);
	}
}

void Task::setStatus(taskStatus stat)
{
	status = stat;

	SMARTS->log(LOG_TASK_STATUS_CHANGE, "%d;%d", id, stat);
}

int Task::getProperty(TaskProps prop)
{
	auto it = properties.find(prop);
	if(it != properties.end())
		return it->second;
	else
		return PROP_NO_VAL;
}

int Task::setProperty(TaskProps prop, int val)
{
	int old = PROP_NO_VAL;

	auto it = properties.find(prop);
	if(it != properties.end())
		old = it->second;

	if(val == PROP_NO_VAL)
		properties.erase(prop);
	else
		properties[prop] = val;

	SMARTS->log(LOG_TASK_PROP_SET, "%d;%d;%d", id, prop, val);

	return old;
}

void Task::reDeclare()
{
	for (int i=0; i<stackBackupSize; i++)
	{
		*(stackBackupSource + i) = *(stackBackupDest + i);
	}

	taskPtr = (void*)stackBackupSource;
	priority = origPriority;
	//status = READY;
	expectedEvent = NULL;
	CSOff = false;
	cyclesCount--;
	leftCyclePeriod = cyclePeriod;
	SMARTS->log(LOG_REDECLARE, "%u", id);
}

unsigned int Task::getLeftCyclePeriod()
{
	return leftCyclePeriod;
}

void Task::leftCyclePeriodDecr()
{
	if (leftCyclePeriod > 0)
	{
		leftCyclePeriod--;
	}
}

unsigned int Task::getcyclesCount()
{
	return cyclesCount;
}

void Task::cyclesCountDecr()
{
	if (cyclesCount > 0)
	{
		cyclesCount--;
	}
}