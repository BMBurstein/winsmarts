#pragma once
#ifndef TASK_H
#define TASK_H

#include "Event.h"
#include "asm.h"
#include "Log.h"

#include <string>
#include <cstddef>

#include <sstream>
#include <cstdarg>

enum taskStatus { READY, NOT_ACTIVE, SUSPENDED, SLEEPING };

const unsigned int WINSMARTS_MAX_PRIORITY = -1;

class WinSMARTS;

class Task
{
private:
  char         stack[65536];           // must be first to align stack
  TaskObj      taskPtr;                // Context of this task
  bool         CSOff;                  // true if this task disabled CS
  unsigned int priority;
  unsigned int const    origPriority;  // for reseting priority
  taskStatus   status;
  size_t        id;
  std::string  name;
  unsigned int sleepCounter;
  Event*       expectedEvent;
  Log&         logger;                 // Output log receiver
  int          logCount;               // Line counter for logger

  char* taskStatusToSting(taskStatus e);
  Task(Task const &);            //   / Not implemented. Prevents copying
  Task& operator=(Task const &); //   \ Copying a TaskObj is dangerous !!

public:
  Task(TaskProc fn, size_t id, std::string const &name, int priority, TaskProc taskEnd, WinSMARTS*, Log& logger, int &logCount);

  void log(std::string const& evt, std::string const& msg = "") ;  //Log function for string
  void log(char const* evt, char const* msg = "", ...);            //Log function for char*

  void sleepDecr(); // decrease sleep counter
  void setStatus(taskStatus stat);

  // accessors
  std::string getName()  const { return name; }
  int getPriority()      const { return priority; }
  taskStatus getStatus() const { return status; }
  bool getCS()           const { return CSOff; }

  // mutators
  void incrPriority()              { if(priority) --priority; }
  void decrPriority()              { if(priority<WINSMARTS_MAX_PRIORITY) ++priority; }
  void setPriority(unsigned int p) { if(p<WINSMARTS_MAX_PRIORITY) priority=p; }
  void restorePriority()           { priority = origPriority; }              // reset priority back to original
  void setSleep(int t)             { sleepCounter = t; }
  void setCS(bool cs)              { CSOff = cs; }

  Event* getExpectedEvent(){ return expectedEvent; }
  void setExpectedEvent(Event* expectedEventp){ expectedEvent = expectedEventp; }

  void switchFrom(TaskObj &tsk) { contextSwitch(&tsk, taskPtr); } // contextSwitch from tsk to this task instance
  void switchTo(TaskObj tsk)    { contextSwitch(&taskPtr, tsk); } // contextSwitch from this task instance to tsk
};

inline void Task::log(std::string const& evt, std::string const& msg)
{
  //??contextSwitchAllow = false;
  
  std::stringstream ss;
  ss << evt << ';' << logCount++ << ';' << msg;
  logger.log(ss.str().c_str(), ss.str().length());
  //??contextSwitchAllow = true;
}

inline void Task::log(char const* evt, char const* msg, ...)
{
  char buffer[256];
  std::va_list args;
  va_start(args, msg);
  vsnprintf(buffer, 256, msg, args);
  va_end(args);

  //??contextSwitchAllow = false;
  
  std::stringstream ss;
  ss << evt << ';' << logCount++ << ';' << buffer;
  logger.log(ss.str().c_str(), ss.str().length());
  //??contextSwitchAllow = true;
}
#endif // TASK_H