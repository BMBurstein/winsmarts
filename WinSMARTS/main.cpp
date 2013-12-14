#include <iostream>
#include "debugger.h"
#include "LogUDP.h"
#include "LogFile.h"
#include "WinSMARTS.h"
using namespace std;


LogUDP Lg;
//LogFile Lg("Output.txt");
//WinSMARTS SMARTS(RoundRobin, Lg, 55);
//WinSMARTS SMARTS(ByPriority, Lg, 55);
WinSMARTS SMARTS(EDF, Lg, 55);
Event e(&SMARTS);


/********************************************************/

void BusyWait(long long j)
{
	for(volatile long long i=0; i<j; i++);
}

void __stdcall a(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'A';
		SMARTS->contextSwitchOn();
		BusyWait(4000000);
	}
}

void __stdcall b(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'B';
		SMARTS->contextSwitchOn();
		BusyWait(4000000);
	}
}

void __stdcall c(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'C';
		SMARTS->contextSwitchOn();
		BusyWait(4000000);
	}
}

void __stdcall D(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'D';
		SMARTS->contextSwitchOn();
		BusyWait(4000000);
	}
}

void __stdcall E(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'E';
		SMARTS->contextSwitchOn();
		BusyWait(4000000);
	}
}
/********************************************************/

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);  // cancel buffering on screen printing

	Debugger dbg(&SMARTS);
	dbg.start();

	//WinSMARTS SMARTS(RoundRobin, Lg, 55);      // instance of our system


	//SMARTS.declareTask(a, "a", 3);    //
	//SMARTS.declareTask(b, "b", 7);    // declare few tasks
	//SMARTS.declareTask(c, "c", 7);    //

	//SMARTS.declareTask(D, "D", 5);
	//SMARTS.declareTask(E, "E", 6);

	SMARTS.declareTask(a, "A", 3, 90, 3);		//
	SMARTS.declareTask(b, "B", 7, 170, 2);		//
	SMARTS.declareTask(c, "C", 7, 210, 2);		// declare a few tasks
	SMARTS.declareTask(D, "D", 5, 150, 1);		//
	SMARTS.declareTask(E, "E", 6, 120, 2);		//

	SMARTS.runTheTasks();        // start running the tasks

	return 0;
}