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



int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);	// cancel buffering on screen printing

	RR rr;								// scheduler instance
	WinSMARTS SMARTS(rr, 55);			// instance of our system
	

	SMARTS.declareTask(a, "a", 5);		//
	SMARTS.declareTask(b, "b", 5);		// declare few tasks
	SMARTS.declareTask(c, "c", 5);		//

	SMARTS.runTheTasks();				// start running the tasks

	return 0;
}