#include "WinSMARTS.h"
#include "schedAlgo.h"

#include <climits>
#include <ctime>
#include <cstdarg>
#include <cstdio>
#include <sstream>
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
		// run at a timer interrupt, invoke every 'timerInterval' milliseconds
		((WinSMARTS*)param)->timerHandler();
	}

	void __stdcall systemIdle(WinSMARTS* param)
	{
		//Waste of time, when there is only task(s) sleepy
		param->systemIdle();
	}
}


WinSMARTS::WinSMARTS(SchedAlgo* scheduler, Log& logger, unsigned int interval)
	: timerInterval(interval),
	algo(scheduler),
	contextSwitchAllow(true),
	endOfTimeSlice(false),
	ranAll(false),
	currentTask(0),
	logger(logger),
	logCount(0),
	debug(false),
	pause(false)
{
	logger.clear();
	// create a task of a waste of time when there is a sleepy task
	//tasks.push_back(unique_ptr<Task>(new Task(::systemIdle, "System Idle", INT_MAX, ::taskEnd, this)));
	declareTask(::systemIdle, "System Idle", WINSMARTS_MAX_PRIORITY);
}

void WinSMARTS::runTheTasks()
{
	timer = setSigTimer(timerInterval, ::timerHandler, this);    // generates a signal every 'timerInterval' milliseconds

	int nextTask;
	while(!ranAll)
	{
		contextSwitchAllow = false;
		nextTask = algo(this);                // decide which task will run now

		log(LOG_CONTEXT_SWITCH, "%u", getCurrentTask());
		setCurrentTask(nextTask);

		if(nextTask == 0 && !isTaskSleeping())
		{
			deadlock = true;
			break;
		}

		breakForDebug();

		if(tasks[getCurrentTask()]->getCS())
			contextSwitchOff();
		else
			contextSwitchAllow = true;
		tasks[getCurrentTask()]->switchFrom(myContext);    // ContextSwitch-> goes to the selectesd task 
	}

	stopSigTimer(timer);
}

tid_t WinSMARTS::declareTask(TaskProc fn, std::string const &name, unsigned int priority)
{
	tasks.push_back(unique_ptr<Task>(new Task(fn, tasks.size(), name, priority, ::taskEnd, this)));
	currentTask = tasks.size() - 1;

	log(LOG_NEW_TASK, "%u;%s;%u", currentTask, name.c_str(), priority);

	return currentTask;
}

void WinSMARTS::taskEnd()
{
	// Called from ::taskEnd
	setTaskStatus(NOT_ACTIVE);
	contextSwitchOn();
	callScheduler();
}

void WinSMARTS::timerHandler()
{
	// Called from ::timerHandler
	log(LOG_TIMER);
	if(getContextSwitch())                // if Context Switch is enabled
		tasks[getCurrentTask()]->switchTo(myContext);  // ContextSwitch-> goes to the 'runTheTasks' function
	else
	{
		endOfTimeSlice = true; // mark exceeded of the time
		breakForDebug();
	}
}

void WinSMARTS::systemIdle()
{
	// Called from ::systemIdle
	while(isTaskSleeping())
		;
	ranAll = true;
}

void WinSMARTS::contextSwitchOn()
{
	log(LOG_CONTEXT_SWITCH_ON);
	tasks[getCurrentTask()]->setCS(false);
	contextSwitchAllow = true;

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

//WinSMARTS::TaskRef WinSMARTS::getTasksByStatus(taskStatus stat)
//{
//	TaskRef TaskList;
//
//	for(tid_t i = 0; i < tasks.size(); ++i)
//		if(tasks[i]->getStatus() == stat)
//			TaskList.push_back(i);
//
//	return TaskList;
//}

inline void WinSMARTS::contextSwitchOff()
{
	contextSwitchAllow = false;
	tasks[getCurrentTask()]->setCS(true);
	log(LOG_CONTEXT_SWITCH_OFF);
}

void WinSMARTS::log(LogMsg evt, std::string const& msg)
{
	log(evt, msg.c_str());
}

void WinSMARTS::log(LogMsg evt, char const* msg, ...)
{
	contextSwitchAllow = false;

	char buffer[260];
	int len;
	std::va_list args;
	va_start(args, msg);
	len = vsnprintf(buffer+5, 255, msg, args);
	va_end(args);
	
	buffer[0] = (char)evt;
	memcpy(buffer+1, &logCount, 4);

	logger.log(buffer, len + 5);

	contextSwitchAllow = !tasks[getCurrentTask()]->getCS();
}

inline void WinSMARTS::breakForDebug()
{
	if(debug)
	{
		pauseSigTimer(timer);
		pause = true;
		while(pause)
			;
		resumeSigTimer(timer);
	}
}

void WinSMARTS::debugBegin()
{
	if(!debug)
	{
		debug = true;
		pause = false;
		while(pause)
			;
	}
}

void WinSMARTS::debugEnd()
{
	if(debug)
	{
		debug = false;
		pause = false;
	}
}

void WinSMARTS::debugStep()
{
	if(debug)
	{
		pause = false;
	}
}

void WinSMARTS::debugSetCurrentTask(tid_t tid)
{
	if(debug)
	{
		setCurrentTask(tid);
	}
}

void WinSMARTS::debugSetContextSwitch(bool allow)
{
	if(debug)
	{
		if(allow)
			contextSwitchOn();
		else
			contextSwitchOff();
	}
}