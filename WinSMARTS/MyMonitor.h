#include "Monitor.h"

enum State {THINKING, HUNGRY, EATING};

class MyMonitor : public Monitor
{
	State* states;
public:
	MyMonitor (int conditionSize, WinSMARTS* SMARTS_);
	void pickUp(int id);
	void putDown(int id);
	int canEat(int id);
};
