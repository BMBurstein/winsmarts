#include <iostream>
#include "debugger.h"
#include "LogUDP.h"
#include "LogFile.h"
#include "WinSMARTS.h"
using namespace std;


LogUDP Lg;
//LogFile Lg("Output.txt");
//WinSMARTS SMARTS(RoundRobin, Lg, 55);
WinSMARTS SMARTS(ByPriority, Lg, 55);
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
		cout << 'a';
		SMARTS->contextSwitchOn();
		BusyWait(400000);
	}

	SMARTS->sleep(4000);

	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'a';
		SMARTS->contextSwitchOn();
		BusyWait(400000);
	}

}

void __stdcall b(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'b';
		SMARTS->contextSwitchOn();
		BusyWait(4000000);
	}
}

void __stdcall c(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'c';
		SMARTS->contextSwitchOn();
		BusyWait(40000000);
	}
}

void __stdcall D(WinSMARTS * SMARTS)
{
	SMARTS->contextSwitchOff();
	cout << 'D';
	SMARTS->contextSwitchOn();


	for(int i=0; i<30; ++i) BusyWait(4000000);
	for(int i=0; i<30; ++i) {BusyWait(20000000); if (i==15) SMARTS->setTaskStatus(SUSPENDED);}

	e.wait((string)"E");

	for(int i=0; i<30; ++i) BusyWait(4000000);

	SMARTS->contextSwitchOff();
	cout << 'D';
	SMARTS->contextSwitchOn();
}

void __stdcall E(WinSMARTS * SMARTS)
{

	SMARTS->contextSwitchOff();
	cout << 'E';
	SMARTS->contextSwitchOn();

	for(int i=0; i<30; ++i) BusyWait(20000000);
	for(int i=0; i<30; ++i) {BusyWait(20000000); }//if (i==15) SMARTS->setTaskStatus(SUSPENDED);}

	SMARTS->contextSwitchOff();
	cout << 'E';
	SMARTS->contextSwitchOn();
	e.send("D",NULL,false);
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

	SMARTS.declareTask(a, "a", 3, 525, 2);    //
	SMARTS.declareTask(b, "b", 7, 80, 2);    // declare few tasks
	SMARTS.declareTask(c, "c", 7, 20000, 1);    //

	SMARTS.declareTask(D, "D", 5, 20000, 1);
	SMARTS.declareTask(E, "E", 6, 20000, 1);

	SMARTS.runTheTasks();        // start running the tasks

	return 0;
}