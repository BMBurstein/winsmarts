#include "WinSMARTS.h"
#include <climits>
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
    // run at a timer interrupt, invoke every 'timerInterval' milliseconds
    ((WinSMARTS*)param)->timerHandler();
  }

  void __stdcall systemIdle(WinSMARTS* param)
  {
    //Waste of time, when there is only task(s) sleepy
    param->systemIdle();
  }
}


WinSMARTS::WinSMARTS(SchedAlgo* scheduler, unsigned int interval)
  : timerInterval(interval),
    algo(scheduler),
    contextSwitchAllow(true),
    endOfTimeSlice(false),
    ranAll(false),
    currentTask(0)
{
  // create a task of a waste of time when there is a sleepy task
  tasks.push_back(unique_ptr<Task>(new Task(::systemIdle, "System Idle", INT_MAX, ::taskEnd, this)));
}

void WinSMARTS::runTheTasks()
{
  void* timer = setSigTimer(timerInterval, ::timerHandler, this);    // generates a signal every 'timerInterval' milliseconds

  int nextTask;
  while(!ranAll)
  {
    nextTask = algo(this);                // decide which task will run now

    setCurrentTask(nextTask);
    if(tasks[getCurrentTask()]->getCS())
      contextSwitchOff();
    tasks[getCurrentTask()]->switchFrom(myContext);    // ContextSwitch-> goes to the selectesd task 
  }

  stopSigTimer(timer);
}

tid_t WinSMARTS::declareTask(TaskProc fn, std::string const &name, int priority)
{
  tasks.push_back(unique_ptr<Task>(new Task(fn, name, priority, ::taskEnd, this)));
  currentTask = tasks.size() - 1;
  return currentTask;
}

void WinSMARTS::taskEnd()
{
  // Continue from taskEnd stdcall
  setTaskStatus(NOT_ACTIVE);
  contextSwitchOn();
  callScheduler();
}

void WinSMARTS::timerHandler()
{
  // Continue from timerHandler stdcall
  if(getContextSwitch())                // if Context Switch is enabled
    tasks[(getCurrentTask())]->switchTo(myContext);  // ContextSwitch-> goes to the 'runTheTasks' function
  else
    endOfTimeSlice = true; // mark exceeded of the time
}

void WinSMARTS::systemIdle()
{
  // Continue from systemIdle stdcall
  while(isTaskSleeping())
    ;
  ranAll = true;
}

void WinSMARTS::contextSwitchOn()
{
  contextSwitchAllow = true;
  tasks[getCurrentTask()]->setCS(false);

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
  for(TaskIt it = tasks.begin(); it != tasks.end(); ++it) //?? tasks.end() is the last task of one task after (=Null)?
    if((*it)->getStatus() == SLEEPING)
      return true;
  return false;
}