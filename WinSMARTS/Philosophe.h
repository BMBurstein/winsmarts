#include "WinSMARTS.h"
#include "MyMonitor.h"

class Philosophe
{
	WinSMARTS* SMARTS;
	MyMonitor* myMonitor;
	int id;
	void busyWait (int t);
	int thinkingTime;
	int eatingTime;

public:    
	Philosophe(int id_, MyMonitor* myMonitor_, int thinkingTime_, int eatingTime_);
	void run(WinSMARTS* SMARTS_);
};
