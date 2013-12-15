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
	debug(false),
	pause(false)
{
	logger.clear();
	// create a task of a waste of time when there is a sleepy task
	declareTask(::systemIdle, "System Idle", WINSMARTS_MAX_PRIORITY, MIN_STACK_SIZE);
}

void WinSMARTS::runTheTasks()
{
	timer = setSigTimer(timerInterval, ::timerHandler, this);    // generates a signal every 'timerInterval' milliseconds

	size_t nextTask;
	while(!ranAll)
	{
		contextSwitchAllow = false;
		nextTask = algo(states, getCurrentTask(), this);                // decide which task will run now

		log(LOG_CONTEXT_SWITCH, "%u", getCurrentTask());
		setCurrentTask(nextTask);

		if(nextTask == 0 && !isTaskSleeping() && isMoreThanOneTaskAlive())
		{
			log(LOG_DEADLOCK, "");
			deadlock = true;
			break;
		}

		breakForDebug();

		if(tasks[getCurrentTask()]->getCSOff())
			contextSwitchOff();
		else
			contextSwitchAllow = true;

		setTaskStatus(RUNNING);
		contextSwitch(&myContext, tasks[getCurrentTask()]->taskPtr);    // ContextSwitch-> goes to the selectesd task 
	}

	stopSigTimer(timer);

	log(LOG_END, "");
}

tid_t WinSMARTS::declareTask(TaskProc fn, std::string const &name, unsigned int priority, size_t stackSize)
{
	contextSwitchAllow = false;
	tasks.push_back(shared_ptr<Task>(new Task(fn, tasks.size(), name, priority, ::taskEnd, this, stackSize)));
	states[READY].insert(tasks.size() - 1);
	contextSwitchAllow = !tasks[getCurrentTask()]->CSOff;

	currentTask = tasks.size() - 1;

	log(LOG_NEW_TASK, "%u;%s;%u", currentTask, name.c_str(), priority);

	return currentTask;
}

void WinSMARTS::taskEnd()
{
	// Called from ::taskEnd
	contextSwitchOn();
	callScheduler(NOT_ACTIVE);
}

void WinSMARTS::timerHandler()
{
	// Called from ::timerHandler
	log(LOG_TIMER);

	for(unsigned int i=0; i<tasks.size(); i++)
		tasks[i]->sleepDecr();

	if(getContextSwitchAllow())                // if Context Switch is enabled
	{
		callScheduler();
	}
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
	callScheduler(SLEEPING);
}

bool WinSMARTS::isTaskSleeping()
{
	return !states[SLEEPING].empty();
}

bool WinSMARTS::isMoreThanOneTaskAlive()
{
	return states[NOT_ACTIVE].size() != tasks.size() - 1;
}

inline void WinSMARTS::contextSwitchOff()
{
	contextSwitchAllow = false;
	tasks[getCurrentTask()]->setCS(true);
	log(LOG_CONTEXT_SWITCH_OFF);
}

void WinSMARTS::setTaskStatus(tid_t tid, taskStatus stat)
{
	taskStatus old = tasks.at(tid)->getStatus();

	contextSwitchAllow = false;
	tasks[tid]->setStatus(stat);
	states[old].erase(tid);
	states[stat].insert(tid);
	contextSwitchAllow = !tasks[getCurrentTask()]->CSOff;
}

void WinSMARTS::log(LogMsg type, std::string const& msg)
{
	log(type, msg.c_str());
}

void WinSMARTS::log(LogMsg type, char const* msg, ...)
{
	contextSwitchAllow = false;

	char buffer[255];
	int len;
	va_list args;
	va_start(args, msg);
	len = vsnprintf(buffer, 255, msg, args);
	va_end(args);

	logger.log(type, buffer, len);

	contextSwitchAllow = !tasks[getCurrentTask()]->getCSOff();
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

//debugging functions. MUST BE CALLED FROM DIFFERENT THREAD!
void WinSMARTS::debugBegin()
{
	if(!debug)
	{
		debug = true;
		pause = false;
		while(!pause)
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