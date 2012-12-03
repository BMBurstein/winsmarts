#pragma once
#ifndef TASK_H
#define TASK_H

#include "common.h"
#include <string>

enum taskStatus { READY, NOT_ACTIVE, SUSPENDED, SLEEPING };

class WinSMARTS;

class Task
{
private:
	taskAsm taskPtr;
	int priority;
	int origPriority;
	taskStatus status;
	std::string name;
	char stack[8192];

public:
	Task(taskProc fn, std::string const &name, int priority, taskProc taskEnd, WinSMARTS*);
	~Task(void);

	void sleepDecr();

	std::string getName() const { return name; }
	int getPriority() const { return priority; }
	void incrPriority() { --priority; }
	void resetPriority() { priority = origPriority; }
	taskStatus getStatus() const { return status; }
	void setStatus(taskStatus stat) { status = stat; }

};

#endif // TASK_H