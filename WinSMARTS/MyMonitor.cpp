#include "MyMonitor.h"

MyMonitor::MyMonitor (int conditionSize, WinSMARTS* SMARTS_):Monitor(conditionSize, SMARTS_)
{
	
	states = new State[conditionSize];
	for(int i=0; i < conditionSize; i++)
		states[i] = THINKING;
}

void MyMonitor::pickUp(int id)
{
	entryM();
	states[id]=HUNGRY;

	if(canEat(id))
		states[id]=EATING;
	else
		WaitCondition(id);		
	exitM();
}

void MyMonitor::putDown(int id)
{
	entryM();
	states[id]=THINKING;
	if(canEat((id+size-1)%size))
	{
		states[(id+size-1)%size] = EATING;
		NotifyCondition((id+size-1)%size);
	}

	if(canEat((id+1)%size))
	{
		states[(id+1)%size] = EATING;
		NotifyCondition((id+1)%size);
	}
	exitM();
}

int MyMonitor::canEat(int id)
{
	return (states[(id+size-1)%size]!=EATING &&
	states[(id+1)%size]!=EATING &&
	states[id]==HUNGRY);
}
