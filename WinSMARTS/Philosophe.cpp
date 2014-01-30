#include "Philosophe.h"
#include <iostream>
using namespace std;

Philosophe::Philosophe(int id_, MyMonitor* myMonitor_, int thinkingTime_, int eatingTime_)
	:id(id_),
	myMonitor(myMonitor_),
	thinkingTime(thinkingTime_),
	eatingTime(eatingTime_)
{ }

void Philosophe::run(WinSMARTS* SMARTS_)
{
	SMARTS = SMARTS_;
	while (true)
	{
		SMARTS->contextSwitchOff();
		cout << "Philosophe " << id << "thinking" << endl;
		SMARTS->contextSwitchOn();
		
		SMARTS->sleep(thinkingTime*500); //thinking
		
		SMARTS->contextSwitchOff();
		cout << "Philosophe " << id << "hungry  " << endl;
		SMARTS->contextSwitchOn();
		
		myMonitor->pickUp(id);
		
		SMARTS->contextSwitchOff();
		cout << "Philosophe " << id << "eating  " << endl;
		SMARTS->contextSwitchOn();
		
		busyWait(eatingTime); //eating
		
		myMonitor->putDown(id);
	}
}


void Philosophe::busyWait (int t)
{
	for(volatile long long i=0; i < (90000000 * t); i++);
}
