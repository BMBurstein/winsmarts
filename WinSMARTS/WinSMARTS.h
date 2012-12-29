#pragma once
#ifndef WINSMARTS_H
#define WINSMARTS_H

#include "common.h"
#include <memory>
#include <string>
#include <vector>
#include "Task.h"
#include "schedAlgo.h"


class WinSMARTS
{
private:
  typedef std::vector<std::unique_ptr<Task> > Tasks; 
  typedef Tasks::iterator TaskIt;

  unsigned int timerInterval;                         // milliseconds between timer pulses
  unsigned int currentTask;                           // index of currently running task
  std::unique_ptr<schedAlgo> algo;                    // schedular algorithim object 
  Tasks tasks;                                        // contains all task for running
  bool deadlock;                                      // turned on when deadlock detected in scheduler
  bool contextSwitchFlag;                             // enables/disables context switch
  bool endOfTimeSlice;                                // turn on when a task has exceeded the interval while context switch is disabled
  bool ranAll;                                        // turn on if all tasks finished
  TaskObj myContext;                                  // Context of runTheTasks() (the scheduelr)

  void setEndOfTimeSlice() { endOfTimeSlice = true; } //Indicates that there is a task that has exceeded the interval 

  WinSMARTS(WinSMARTS const&);
  WinSMARTS& operator=(WinSMARTS const&);

public:
  WinSMARTS(schedAlgo* scheduler, unsigned int interval = 55);

  void runTheTasks();                                                                  // Start running the tasks
  void declareTask(TaskProc fn, std::string const &name, int priority);                // Add a new task to the tasks vector

  void contextSwitchOn();                                                              // Enable context switch
  void sleep(unsigned int ms);                                                         // Send currnet task to sleep
  bool isTaskSleeping();                                                               // Indicates that there is a task is sleeping, and it can't be deadlock

  int getCurrentTask() const { return currentTask; }                                   // Get the current running task index
  void setCurrentTask(int taskNum) { currentTask = taskNum; }                          // Set the current running task index
  size_t getTotalTasks() const { return tasks.size(); }                                   // Get total declared tasks
  bool getDeadlock() const { return deadlock; }                                        // Get deadlock flag
  void setDeadlock() { deadlock = true; }                                              // Turn on deadlock flag
  bool getContextSwitch() const { return contextSwitchFlag; }                          // Get 'context switch' flag status
  void contextSwitchOff() { contextSwitchFlag = false; }                               // Disable context switch
  std::string getName(int taskNum) const { return tasks.at(taskNum)->getName(); }      // Get task's name by it's index
  std::string getName() const { return getName(getCurrentTask()) ; }                   // Get current task's name
  taskStatus getStatus(int taskNum) const { return tasks.at(taskNum)->getStatus(); }   // Get task's status by it's index
  taskStatus getStatus() const { return getStatus(getCurrentTask()); }                 // Get current task's status
  void setStatus(int taskNum, taskStatus stat) { tasks.at(taskNum)->setStatus(stat); } // Set task's status by it's index
  void setStatus(taskStatus stat) { setStatus(getCurrentTask(), stat); }               // Set current task's status
  int getPriority(int taskNum) const { return tasks.at(taskNum)->getPriority(); }      // Get task's priority by it's index
  int getPriority() const { return getPriority(getCurrentTask()); }                    // Get current task's status
  void incrPriority(int taskNum) { tasks.at(taskNum)->incrPriority(); }                // Increase task's priority
  void resetPriority(int taskNum) { tasks.at(taskNum)->resetPriority(); }              // Decrease task's priority
  void sleepDecr(int taskNum) { tasks.at(taskNum)->sleepDecr(); }                      // Decrease task's sleep time
  void callScheduler() { timerHandler(); }                                             // return the control to the scheduler

  void taskEnd();
  void timerHandler();
  void systemIdle();
};

#endif // WINSMARTS_H