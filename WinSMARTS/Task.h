#pragma once
#ifndef TASK_H
#define TASK_H

#include "platform.h"
#include "Event.h"
#include "asm.h"
#include "Log.h"

#include <string>
#include <cstddef>

#include <sstream>
#include <cstdarg>

//! The type used for task ids
typedef size_t tid_t;
const tid_t NO_TASK = (tid_t)-1;

enum taskStatus { READY, NOT_ACTIVE, SUSPENDED, SLEEPING, RUNNING, NUM_OF_STATUSES };

const unsigned int WINSMARTS_MAX_PRIORITY = -1;

class WinSMARTS;

class Task
{
private:
	char         *stack;
	contextHnd   taskPtr;                // Context of this task
	bool         CSOff;                  // true if this task disabled CS
	unsigned int priority;
	unsigned int const    origPriority;  // for reseting priority
	taskStatus   status;
	tid_t        id;
	std::string  name;
	unsigned int sleepCounter;
	Event*       expectedEvent;
	int          logCount;               // Line counter for logger
	WinSMARTS*   SMARTS;
	
	char*        stackBackupSource;      // stack head after creation
	char*        stackBackupDest;        // backup stack head
	unsigned int          stackBackupSize;        // backup stack head
	unsigned int          cyclePeriod;            // store the original period time
	unsigned int          leftCyclePeriod;        // store the current period time
	unsigned int          cyclesCount;            // store the current remained cycles

	Task(Task const &);            //   / Not implemented. Prevents copying
	Task& operator=(Task const &); //   \ Copying a TaskObj is dangerous !!

	friend WinSMARTS;


	
public:
	Task(TaskProc fn, size_t id_, std::string const &name_, int priority_, TaskProc taskEnd, WinSMARTS* SMARTS_, unsigned int cyclePeriod_, unsigned int cyclesCount_, size_t stackSize);
	~Task();

	void sleepDecr(); // decrease sleep counter
	void setStatus(taskStatus stat);

	// accessors
	std::string getName()  const { return name; }
	int getPriority()      const { return priority; }
	taskStatus getStatus() const { return status; }
	bool getCSOff()           const { return CSOff; }

	// mutators
	void incrPriority()              { if(priority) --priority; }
	void decrPriority()              { if(priority<WINSMARTS_MAX_PRIORITY) ++priority; }
	void setPriority(unsigned int p) { if(p<WINSMARTS_MAX_PRIORITY) priority=p; }
	void restorePriority()           { priority = origPriority; }              // reset priority back to original
	void setSleep(int t)             { sleepCounter = t; }
	void setCS(bool cs)              { CSOff = cs; }

	Event* getExpectedEvent(){ return expectedEvent; }
	void setExpectedEvent(Event* expectedEventp){ expectedEvent = expectedEventp; }
	unsigned int getLeftCyclePeriod();
	void leftCyclePeriodDecr();
	unsigned int getcyclesCount();
	void cyclesCountDecr();
	void reDeclare();

	static char* taskStatusToString(taskStatus e);
};
#endif // TASK_H