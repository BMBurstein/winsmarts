#pragma once
#ifndef WINSMARTS_H
#define WINSMARTS_H

#include "common.h"
#include <string>
#include <vector>
#include "Task.h"


class schedAlgo;

class WinSMARTS
{
private:
	unsigned int timerInterval; // milliseconds between timer pulses
	unsigned int currentTask;
	schedAlgo * algo;
	std::vector<Task> tasks;
	bool deadlock, contextSwitchFlag;

public:
	WinSMARTS();
	~WinSMARTS();

	void runTheTasks();
	void declareTask(taskProc fn, std::string const &name, int priority);

	void callScheduler();
	void setCurrentTask(unsigned int);
	bool contextSwitchOn();
	bool contextSwitchOff();
	void sleep(unsigned int ms);

	int getCurrentTask() const { return currentTask; }
	int getTotalTasks() const { return tasks.size(); }
	bool getDeadlock() const { return deadlock; }
	void setDeadlock() { deadlock = true; }
	bool getContextSwitch() const { return contextSwitchFlag; }
	std::string getName(int taskNum) const { return tasks.at(taskNum).getName(); }
	std::string getName() const { return getName(getCurrentTask()) ; } // Get current task's name
	taskStatus getStatus(int taskNum) const { return tasks.at(taskNum).getStatus(); }
	taskStatus getStatus() const { return getStatus(getCurrentTask()); } // Get current task's status
	void setStatus(int taskNum, taskStatus stat) { tasks.at(taskNum).setStatus(stat); }
	void setStatus(taskStatus stat) { setStatus(getCurrentTask(), stat); } // Set current task's status
	int getPriority(int taskNum) const { return tasks.at(taskNum).getPriority(); }
	int getPriority() const { return getPriority(getCurrentTask()); } // Get current task's status
	void incrPriority(int taskNum) { tasks.at(taskNum).incrPriority(); }
	void resetPriority(int taskNum) { tasks.at(taskNum).resetPriority(); }
	void sleepDecr(int taskNum) { tasks.at(taskNum).sleepDecr(); }

	void taskEnd();
};

#endif // WINSMARTS_H