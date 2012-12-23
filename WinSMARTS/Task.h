#pragma once
#ifndef TASK_H
#define TASK_H

#include "common.h"
#include <string>
#include <cstddef>

enum taskStatus { READY, NOT_ACTIVE, SUSPENDED, SLEEPING };

class WinSMARTS;

class Task
{
private:
	TaskAsm taskPtr;		// ??
	unsigned int priority;
	int origPriority;
	taskStatus status;
	std::string name;
	char stack[65536];		// ?? did this stack contains thread's registers and flags?
	int sleepCounter;

	Task(Task const &);
	Task& operator=(Task const &);

public:
	Task(TaskProc fn, std::string const &namep, int priorityp, TaskProc taskEnd, WinSMARTS*);

	void sleepDecr();

	std::string getName() const { return name; }
	int getPriority() const { return priority; }
	void incrPriority() { if(priority) --priority; }
	void resetPriority() { priority = origPriority; }
	taskStatus getStatus() const { return status; }
	void setStatus(taskStatus stat) { status = stat; }
	void setSleep(int t) { sleepCounter = t; }

	void switchFrom(TaskAsm &tsk) { contextSwitch(&tsk, taskPtr); }		// ??
	void switchTo(TaskAsm tsk) { contextSwitch(&taskPtr, tsk); }		// ??
};

#endif // TASK_H