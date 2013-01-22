#include <iostream>
#include "WinSMARTS.h"
using namespace std;

#pragma optimize("", off)
void BusyWait(int j)
{
	for(int i=0; i<j; i++);
}
#pragma optimize("", on)

void __stdcall a(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'a';
		SMARTS->contextSwitchOn();
		BusyWait(40000000);
	}
}

void __stdcall b(WinSMARTS * SMARTS)
{
	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'b';
		SMARTS->contextSwitchOn();
		BusyWait(40000000);
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





/********************************************************/
WinSMARTS SMARTS(RoundRobin, 55);
Event e(&SMARTS);
void __stdcall D(WinSMARTS * SMARTS)
{
	e.wait((string)"E");//*****

	SMARTS->contextSwitchOff();
	cout << 'D';
	SMARTS->contextSwitchOn();
	for(int i=0; i<30; ++i) BusyWait(40000000);
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
	SMARTS->contextSwitchOff();
	cout << 'E';
	SMARTS->contextSwitchOn();

	e.send("D",NULL,false); //*****
}
/********************************************************/

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);	// cancel buffering on screen printing

	//WinSMARTS SMARTS(RoundRobin, 55);			// instance of our system
	

	//SMARTS.declareTask(a, "a", 5);		//
	//SMARTS.declareTask(b, "b", 5);		// declare few tasks
	//SMARTS.declareTask(c, "c", 5);		//

	SMARTS.declareTask(D, "D", 5);
	SMARTS.declareTask(E, "E", 5);
    
	SMARTS.runTheTasks();				// start running the tasks

	return 0;
}