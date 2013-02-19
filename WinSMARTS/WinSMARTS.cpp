#include "WinSMARTS.h"
#include <climits>
#include "timer.h"
#include "schedAlgo.h"
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
    logCount(0)
{
  declareTask(::systemIdle, "System Idle", WINSMARTS_MAX_PRIORITY);
}

void WinSMARTS::runTheTasks()
{
  void* timer = setSigTimer(timerInterval, ::timerHandler, this);    // generates a signal every 'timerInterval' milliseconds

  int nextTask;
  while(!ranAll)
  {
    contextSwitchAllow = false;
    nextTask = algo(this);                // decide which task will run now

    log("CS", "%d", getCurrentTask());
    setCurrentTask(nextTask);

    if(nextTask == 0 && !isTaskSleeping())
    {
      setDeadlock();
      break;
    }

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
  tasks.push_back(unique_ptr<Task>(new Task(fn, name, priority, ::taskEnd, this)));
  currentTask = tasks.size() - 1;

  stringstream ss;
  ss << currentTask << ';' << name << ';' << priority;
  log("DT", ss.str());

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
  log("TM");
  if(getContextSwitch())                // if Context Switch is enabled
    tasks[(getCurrentTask())]->switchTo(myContext);  // ContextSwitch-> goes to the 'runTheTasks' function
  else
    endOfTimeSlice = true; // mark exceeded of the time
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
  log("CSOn");
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
  for(TaskIt it = tasks.begin(); it != tasks.end(); ++it)
    if((*it)->getStatus() == SLEEPING)
      return true;
  return false;
}

WinSMARTS::TaskRef WinSMARTS::getTasksByStatus(taskStatus stat)
{
  TaskRef TaskList;
  tid_t tid;
  for(tid_t i = 0; i < tasks.size(); ++i)
    if(tasks[i]->getStatus() == stat)
      TaskList.push_back(i);

  return TaskList;
}