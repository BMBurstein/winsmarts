#include "stdafx.h"
#include "WinSMARTS.h"
using namespace std;

void __stdcall a(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		cout << 'a';
		for(long int j=0; j<50000000; ++j)
			;
	}
}

void __stdcall b(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		cout << 'b';
		for(long int j=0; j<50000000; ++j)
			;
	}
}

void __stdcall c(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{

		cout << 'c';
		for(long int j=0; j<50000000; ++j)
			;
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
		for(int count=1; smarts->getStatus(nextTask) != READY && count<smarts->getTotalTasks(); ++count)
		{
			if(++nextTask == smarts->getTotalTasks())
				nextTask = 1;
		}
		if(smarts->getStatus(nextTask) != READY)
			nextTask = 0;
		return nextTask;
	}

	RR* clone() const { return new RR(*this); }
};

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);

	RR rr;
	WinSMARTS SMARTS(rr, 10);
	
	SMARTS.declareTask(a, "a", 5);
	SMARTS.declareTask(b, "b", 5);
	SMARTS.declareTask(c, "c", 5);
	SMARTS.runTheTasks();

	return 0;
}