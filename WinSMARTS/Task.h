#pragma once
#ifndef TASK_H
#define TASK_H

#include "Event.h"
#include "asm.h"
#include <string>
#include <cstddef>

enum taskStatus { READY, NOT_ACTIVE, SUSPENDED, SLEEPING };

class WinSMARTS;

class Task
{
private:
	TaskObj taskPtr;		// Context of this task
	unsigned int priority;
	int origPriority;
	taskStatus status;
	std::string name;
	char stack[65536];
	int sleepCounter;
	Event* expectedEvent; 

	Task(Task const &);
	Task& operator=(Task const &);

public:
	Task(TaskProc fn, std::string const &namep, int priorityp, TaskProc taskEnd, WinSMARTS*);

	void sleepDecr();

	std::string getName() const { return name; }
	int getPriority() const { return priority; }
	void incrPriority() { if(priority) --priority; }
	void setOriginalPriority() { priority = origPriority; }
	taskStatus getStatus() const { return status; }
	void setStatus(taskStatus stat) { status = stat; }
	void setSleep(int t) { sleepCounter = t; }
	Event* getExpectedEvent(){ return expectedEvent; }
	void setExpectedEvent(Event* expectedEventp){ expectedEvent = expectedEventp; }

	void switchFrom(TaskObj &tsk) { contextSwitch(&tsk, taskPtr); } // contextSwitch from schedular to this task instance
	void switchTo(TaskObj tsk) { contextSwitch(&taskPtr, tsk); }	// contextSwitch from this task instance to schedular
};

#endif // TASK_H