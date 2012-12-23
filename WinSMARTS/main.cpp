#include "stdafx.h"
#include "WinSMARTS.h"
using namespace std;

#pragma optimize("", off)
void BusyWait(int j, char c)
{
	cout << c;
	for(int i=0; i<j; i++);
}
#pragma optimize("", on)

void __stdcall a(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		BusyWait(50000000, 'a');
	}
}

void __stdcall b(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		BusyWait(50000000, 'b');
	}
}

void __stdcall c(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		BusyWait(50000000, 'c');
	}
}

class RR : public schedAlgo
{
public:
	int schedule() const
	{
		int nextTask = smarts->getCurrentTask() + 1;
		if(nextTask == smarts->getTotalTasks())
			nextTask = 1;

		//passes on all the tasks array and checks if there is ready task 
		for(int count=1; smarts->getStatus(nextTask) != READY && count<smarts->getTotalTasks(); ++count)
		{
			if(++nextTask == smarts->getTotalTasks())
				nextTask = 1;
		}

		//if not found any ready task, go back to ??
		if(smarts->getStatus(nextTask) != READY)
			nextTask = 0;
		return nextTask;
	}

	RR* clone() const { return new RR(*this); }
};

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);	// cancel buffering on screen printing

	RR rr;								// scheduler instance
	WinSMARTS SMARTS(rr, 10);			// instance of our system
	

	SMARTS.declareTask(a, "a", 5);		//
	SMARTS.declareTask(b, "b", 5);		// declare few tasks
	SMARTS.declareTask(c, "c", 5);		//

	SMARTS.runTheTasks();				// start running the tasks

	return 0;
}