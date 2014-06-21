#include <iostream>
#include "debugger.h"
#include "LogUDP.h"
#include "LogFile.h"
#include "WinSMARTS.h"
using namespace std;

LogUDP Lg;
WinSMARTS SMARTS(RoundRobin, Lg, 55);
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

	e.send("b", NULL, true);
}

void __stdcall b(WinSMARTS * SMARTS)
{
	string s;

	for(int i=0; i<50; ++i)
	{
		SMARTS->contextSwitchOff();
		cout << 'b';
		SMARTS->contextSwitchOn();
		BusyWait(4000000);
	}
	
	SMARTS->contextSwitchOff();
	cout << "\n *** b finished ***\n";
	SMARTS->contextSwitchOn();

	e.wait(s);
	SMARTS->contextSwitchOff();
	cout << "\n *** " << s << " finished ***\n";
	SMARTS->contextSwitchOn();

	e.wait(s);
	SMARTS->contextSwitchOff();
	cout << "\n *** " << s << " finished ***\n";
	SMARTS->contextSwitchOn();
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

	e.send("b", NULL, true);
}
/********************************************************/

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);  // cancel buffering on screen printing

	Debugger dbg(&SMARTS);
	dbg.start();

	//WinSMARTS SMARTS(RoundRobin, Lg, 55);      // instance of our system


	SMARTS.declareTask(a, "1st task", 3);    //
	SMARTS.declareTask(b, "2nd task", 7);    // declare few tasks
	SMARTS.declareTask(c, "3rd task", 7);    //

	SMARTS.runTheTasks();        // start running the tasks

	return 0;
}