#include "StdAfx.h"
#include "WinSMARTS.h"
#include "timer.h"
#include "schedAlgo.h"

using namespace std;


namespace {
	void __stdcall taskEnd(WinSMARTS* param)
	{
		param->taskEnd();
	}

	void __stdcall timerHandler(void* param)
	{
		((WinSMARTS*)param)->timerHandler();
	}

	void __stdcall systemIdle(WinSMARTS* param)
	{
		param->systemIdle();
	}

	void __stdcall idleTaskEnd(WinSMARTS* param)
	{
		param->idleTaskEnd();
	}
}


WinSMARTS::WinSMARTS(schedAlgo& scheduler, unsigned int interval)
	: timerInterval(interval),
	  algo(scheduler.clone()),
		contextSwitchFlag(true),
		endOfTimeSlice(false),
		sleepingTasks(0),
		ranAll(false),
		currentTask(0)
{
	tasks.push_back(unique_ptr<Task>(new Task(::systemIdle, this, "System Idle", MAXINT, ::idleTaskEnd, this)));
	algo->smarts = this;
}

void WinSMARTS::runTheTasks()
{
	setSigTimer(timerInterval, ::timerHandler, this);

	while(!ranAll)
	{
		int nextTask = algo->schedule();

		setCurrentTask(nextTask);
		tasks[(getCurrentTask())]->switchFrom(myContext);
	}
}

void WinSMARTS::declareTask(TaskProc fn, std::string const &name, int priority)
{
	//context.emplace_back(fn, name, priority, ::taskEnd);
	tasks.push_back(unique_ptr<Task>(new Task(fn, name, priority, ::taskEnd, this)));
	currentTask = tasks.size() - 1;
}

void WinSMARTS::taskEnd()
{
	setStatus(NOT_ACTIVE);
	contextSwitchOn();
	callScheduler();
}

void WinSMARTS::timerHandler()
{
	if(getContextSwitch())
		tasks[(getCurrentTask())]->switchTo(myContext);
	else
		setEndOfTimeSlice();
}

void WinSMARTS::systemIdle()
{
	while(isTaskSleeping())
		;
}

void WinSMARTS::idleTaskEnd()
{
	ranAll = true;
	taskEnd();
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