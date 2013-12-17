#include <iostream>
#include "debugger.h"
#include "LogUDP.h"
#include "LogFile.h"
#include "WinSMARTS.h"
using namespace std;
#include "Philosophe.h"


LogUDP Lg;
//LogFile Lg("Output.txt");
WinSMARTS SMARTS(RoundRobin, Lg, 55);
//WinSMARTS SMARTS(ByPriority, Lg, 55);
Event e(&SMARTS);


/********************************************************/

MyMonitor* monitor = new MyMonitor(5, &SMARTS);
Philosophe p1(1, monitor, 7, 5);
Philosophe p2(2, monitor, 15, 12);
Philosophe p3(3, monitor, 4, 13);
Philosophe p4(4, monitor, 9, 9);
Philosophe p5(0, monitor, 10, 5);

void __stdcall runp1 (WinSMARTS *) {p1.run(&SMARTS);}
void __stdcall runp2 (WinSMARTS *) {p2.run(&SMARTS);}
void __stdcall runp3 (WinSMARTS *) {p3.run(&SMARTS);}
void __stdcall runp4 (WinSMARTS *) {p4.run(&SMARTS);}
void __stdcall runp5 (WinSMARTS *) {p5.run(&SMARTS);}

/********************************************************/

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);  // cancel buffering on screen printing

	Debugger dbg(&SMARTS);
	dbg.start();

	SMARTS.declareTask(runp1, "P1", 1);
	SMARTS.declareTask(runp2, "P2", 2);
	SMARTS.declareTask(runp3, "P3", 3);
	SMARTS.declareTask(runp4, "P4", 4);
	SMARTS.declareTask(runp5, "P5", 4);


	SMARTS.runTheTasks();        // start running the tasks

	return 0;
}