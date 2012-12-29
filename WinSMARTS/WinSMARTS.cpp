#include "StdAfx.h"
#include "WinSMARTS.h"
#include "timer.h"
#include "schedAlgo.h"

using namespace std;


namespace
{
	void __stdcall taskEnd(WinSMARTS* param)
	{
		// run when the task ended
		param->taskEnd();
	}

	void __stdcall timerHandler(void* param)
	{
		// run every 'timerInterval' milliseconds
		((WinSMARTS*)param)->timerHandler();
	}

	void __stdcall systemIdle(WinSMARTS* param)
	{
		//Waste of time
		param->systemIdle();
	}
}


WinSMARTS::WinSMARTS(SchedAlgo* scheduler, unsigned int interval)
	: timerInterval(interval),
	  algo(scheduler), // creates an instance of the Scheduler
		contextSwitchFlag(true),
		endOfTimeSlice(false),
		ranAll(false),
		currentTask(0)
{
	// create a task of a waste of time when there is a sleepy task
	tasks.push_back(unique_ptr<Task>(new Task(::systemIdle, "System Idle", MAXINT, ::taskEnd, this)));
}

void WinSMARTS::runTheTasks()
{
	void* timer = setSigTimer(timerInterval, ::timerHandler, this);		// generates a signal every 'timerInterval' milliseconds

	int nextTask;
	while(!ranAll)
	{
		nextTask = algo(this);						// decide which thread will run now

		setCurrentTask(nextTask);
		tasks[getCurrentTask()]->switchFrom(myContext);
	}

	stopSigTimer(timer);
}

void WinSMARTS::declareTask(TaskProc fn, std::string const &name, int priority)
{
	tasks.push_back(unique_ptr<Task>(new Task(fn, name, priority, ::taskEnd, this)));
	currentTask = tasks.size() - 1;
}

void WinSMARTS::taskEnd()
{
	//the task ended
	setStatus(NOT_ACTIVE);
	contextSwitchOn();
	callScheduler();
}

void WinSMARTS::timerHandler()
{
	// run at a timer interrupt
	if(getContextSwitch()) // if Context Switch is enabled
		tasks[(getCurrentTask())]->switchTo(myContext);
	else
		setEndOfTimeSlice(); // mark exceeded of the time
}

void WinSMARTS::systemIdle()
{
	// busy wait when there is a sleepy task
	while(isTaskSleeping())
		;
	ranAll = true;
}

void WinSMARTS::contextSwitchOn()
{
	contextSwitchFlag = true;

	if(endOfTimeSlice)
	{
		endOfTimeSlice = false;
		callScheduler();
	}
}
void WinSMARTS::sleep(unsigned int ms)
{
	tasks[getCurrentTask()]->setSleep(ms / timerInterval);
	tasks[getCurrentTask()]->setStatus(SLEEPING);
	callScheduler();
}

bool WinSMARTS::isTaskSleeping()
{
	for(TaskIt it = tasks.begin(); it != tasks.end(); ++it)
		if((*it)->getStatus() == SLEEPING)
			return true;
	return false;
}