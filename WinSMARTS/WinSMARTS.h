#pragma once
#ifndef WINSMARTS_H
#define WINSMARTS_H

#include "common.h"
#include "Task.h"
#include "asm.h"
#include "schedAlgo.h"
#include "Log.h"
#include "timer.h"

#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <cstdarg>

typedef size_t tid_t;

class WinSMARTS
{
private:
  typedef std::vector<std::unique_ptr<Task> > Tasks;
  typedef Tasks::iterator TaskIt;
  typedef std::vector<tid_t> TaskRef;

  Tasks        tasks;              // The task list
  unsigned int timerInterval;      // milliseconds between timer pulses
  tid_t        currentTask;        // index of currently running task
  SchedAlgo*   algo;               // scheduler function
  bool         deadlock;           // true when deadlock detected in scheduler
  bool         contextSwitchAllow; // enables/disables context switch
  bool         endOfTimeSlice;     // indicates a timer interrupt occured while context switch was disabled
  bool         ranAll;             // flag for idle task. true if all tasks finished
  TaskObj      myContext;          // Context of runTheTasks() (the scheduelr)
  Log&         logger;             // Output log receiver
  int          logCount;           // Line counter for logger
  volatile bool         debug;
  volatile bool         pause;
  timerHandle  timer;

  void log(std::string const& evt, std::string const& msg = "") ;  //Log function for string
  void log(char const* evt, char const* msg = "", ...);            //Log function for char*

  WinSMARTS(WinSMARTS const&);            //   / Not implemented. Prevents copying
  WinSMARTS& operator=(WinSMARTS const&); //   \ Copying a TaskObj is dangerous !!

  void breakForDebug();

  friend Task;

public:
  WinSMARTS(SchedAlgo* scheduler, Log& logger, unsigned int interval = 55);

  void runTheTasks();                                                                  // Start running the tasks
  tid_t declareTask(TaskProc fn, std::string const &name, unsigned int priority);      // Add a new task to the tasks vector
  void callScheduler() { tasks[getCurrentTask()]->switchTo(myContext); }               // Return the control to the scheduler

  // Task managment
  void    sleep(unsigned int ms);                                                      // Send currnet task to sleep
  void    contextSwitchOn();                                                           // Enable context switch
  void    contextSwitchOff();                                                          // Disable context switch
  TaskRef getTasksByStatus(taskStatus stat);                                           // Get task list by task status

  bool isTaskSleeping();

  // WinSMARTS accessors
  tid_t       getCurrentTask()           const { return currentTask; }                       // Tid of running task
  size_t      getTotalTasks()            const { return tasks.size(); }                      // Number of declared tasks
  bool        getDeadlock()              const { return deadlock; }                          // True if in deadlock
  bool        getContextSwitch()         const { return contextSwitchAllow; }                // True if context switching allowed
  // Task accessors
  std::string getTaskName(tid_t tid)     const { return tasks.at(tid)->getName(); }          // Get task's name by it's tid
  std::string getTaskName()              const { return getTaskName(getCurrentTask()); }     // Get current task's name
  taskStatus  getTaskStatus(tid_t tid)   const { return tasks.at(tid)->getStatus(); }        // Get task's status by it's tid
  taskStatus  getTaskStatus()            const { return getTaskStatus(getCurrentTask()); }   // Get current task's status
  int         getTaskPriority(tid_t tid) const { return tasks.at(tid)->getPriority(); }      // Get task's priority by it's tid
  int         getTaskPriority()          const { return getTaskPriority(getCurrentTask()); } // Get current task's priority
 
  // Task mutators
  void incrTaskPriority(tid_t tid)               { tasks.at(tid)->incrPriority(); }          // Increase task's priority by tid
  void incrTaskPriority()                        { incrTaskPriority(getCurrentTask()); }     // Increase current task's priority
  void setTaskStatus(tid_t tid, taskStatus stat) { tasks.at(tid)->setStatus(stat); }         // Set task's status by it's tid
  void setTaskStatus(taskStatus stat)            { setTaskStatus(getCurrentTask(), stat); }  // Set current task's status
  void restorePriority(tid_t tid)                { tasks.at(tid)->restorePriority(); }       // Restore task's priority by tid
  void restorePriority()                         { restorePriority(getCurrentTask()); }      // Restore current task's priority

  void setDeadlock()                             { deadlock = true; }                        // Turn on deadlock flag
  void setCurrentTask(tid_t tid)                 { currentTask = tid; }                      // Set tid to be run
  void sleepDecr(tid_t tid)                      { tasks.at(tid)->sleepDecr(); }             // Decrease task's sleep time

  Event* getExpectedEvent(int tid){ return (tid >= 0 && tid <= getTotalTasks())? tasks.at(tid)->getExpectedEvent() : NULL; } //Get task's expectedEvent by it's index
  Event* getCurrentExpectedEvent() { return tasks.at(getCurrentTask())->getExpectedEvent(); }                                // Get current task's expectedEvent
  void setCurrentExpectedEvent(Event* expectedEventp) { tasks.at(getCurrentTask())->setExpectedEvent(expectedEventp); }      // Set current task's expectedEvent

  void taskEnd();
  void timerHandler();
  void systemIdle();

  // Debugging functions
  void debugBegin(); // this must be called before any debug functions!
  void debugEnd();   // call this when done debugging
  void debugStep();
  void debugSetCurrentTask(tid_t tid);
  void debugSetContextSwitch(bool allow);
};



inline void WinSMARTS::contextSwitchOff()
{
  log("ContextSwitch - Off");
  contextSwitchAllow = false;
  tasks[getCurrentTask()]->setCS(true);
}

inline void WinSMARTS::log(std::string const& evt, std::string const& msg)
{
  contextSwitchAllow = false;
  
  std::stringstream ss;
  ss << evt << ';' << logCount++ << ';' << msg;
  logger.log(ss.str().c_str(), ss.str().length());
  contextSwitchAllow = true;
}

inline void WinSMARTS::log(char const* evt, char const* msg, ...)
{
  char buffer[256];
  std::va_list args;
  va_start(args, msg);
  vsnprintf(buffer, 256, msg, args);
  va_end(args);

  contextSwitchAllow = false;
  
  std::stringstream ss;
  ss << evt << ';' << logCount++ << ';' << buffer;
  logger.log(ss.str().c_str(), ss.str().length());
  contextSwitchAllow = true;
}

#endif // WINSMARTS_H