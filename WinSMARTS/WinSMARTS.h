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
	unsigned int timerInterval; // milliseconds between timer pulses
	unsigned int currentTask;
	unsigned int sleepingTasks;
	std::unique_ptr<schedAlgo> algo;
	typedef std::vector<std::unique_ptr<Task> > Tasks;
	typedef Tasks::iterator TaskIt;
	Tasks tasks;
	bool deadlock, contextSwitchFlag, endOfTimeSlice;
	bool ranAll;
	TaskAsm myContext;
	
	void setEndOfTimeSlice() { endOfTimeSlice = true; }

	WinSMARTS(WinSMARTS const&);
	WinSMARTS& operator=(WinSMARTS const&);

public:
	WinSMARTS(schedAlgo& scheduler, unsigned int interval = 55);

	void runTheTasks();
	void declareTask(TaskProc fn, std::string const &name, int priority);

	void contextSwitchOn();
	void sleep(unsigned int ms);

	int getCurrentTask() const { return currentTask; }
	void setCurrentTask(int taskNum) { currentTask = taskNum; }
	int getTotalTasks() const { return tasks.size(); }
	bool getDeadlock() const { return deadlock; }
	void setDeadlock() { deadlock = true; }
	bool getContextSwitch() const { return contextSwitchFlag; }
	void contextSwitchOff() { contextSwitchFlag = false; }
	std::string getName(int taskNum) const { return tasks.at(taskNum)->getName(); }
	std::string getName() const { return getName(getCurrentTask()) ; } // Get current task's name
	taskStatus getStatus(int taskNum) const { return tasks.at(taskNum)->getStatus(); }
	taskStatus getStatus() const { return getStatus(getCurrentTask()); } // Get current task's status
	void setStatus(int taskNum, taskStatus stat) { tasks.at(taskNum)->setStatus(stat); }
	void setStatus(taskStatus stat) { setStatus(getCurrentTask(), stat); } // Set current task's status
	int getPriority(int taskNum) const { return tasks.at(taskNum)->getPriority(); }
	int getPriority() const { return getPriority(getCurrentTask()); } // Get current task's status
	void incrPriority(int taskNum) { tasks.at(taskNum)->incrPriority(); }
	void resetPriority(int taskNum) { tasks.at(taskNum)->resetPriority(); }
	void sleepDecr(int taskNum) { tasks.at(taskNum)->sleepDecr(); }
	bool isTaskSleeping() { return sleepingTasks > 0; }
	void callScheduler() { timerHandler(); }

	void taskEnd();
	void timerHandler();
	void systemIdle();
	void idleTaskEnd();
};

#endif // WINSMARTS_H