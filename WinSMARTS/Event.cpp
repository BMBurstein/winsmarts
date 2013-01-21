#include "Event.h"
#include "WinSMARTS.h"

Event::Event(WinSMARTS* SMARTSp)
{
	SMARTS = SMARTSp;
	reset();
}

void Event::send(std::string name, void *param, int synch)
{
	int i;
	while(testAndSet( )); // while the reciver not read the data yet

	source = SMARTS->getCurrentName( );
	data = param;

	for (i=1; i < SMARTS->getTotalTasks(); i++)
		if (SMARTS->getName(i) == name)
			break;
	if (i < SMARTS->getTotalTasks()) // the target found
	{
		if (SMARTS->getStatus(i)==SUSPENDED && SMARTS->getExpectedEvent(i)==this) // if the target is suspended and it wait for this event
			SMARTS->setStatus(i,READY);

		if (synch)
		{
			senderWaitIndex = SMARTS->getCurrentTask( );
			SMARTS->setCurrentStatus(SUSPENDED);
			dest = name;
			SMARTS->callScheduler( );
		}
		else
			dest = name;
	}
}

void *Event::wait(std::string &sourceP)
{
	void* param;									//delivered data
	if (!flag || dest!=SMARTS->getCurrentName())
	{
		SMARTS->setCurrentExpectedEvent(this);
		SMARTS->setCurrentStatus(SUSPENDED);
		SMARTS->callScheduler();
	}

	sourceP = source;
	param = data;
	if (senderWaitIndex >= 0)
		SMARTS->setStatus(senderWaitIndex,READY);
	reset();
	return (param);
}

bool Event::isArrived(std::string &sourceP) // checked by target, if so get sender name
{
	if (flag)
		sourceP = source;
	return flag;
}

bool Event::reset()
{
	if (flag) 
	{
		flag=false;
		data = NULL;
		source = dest = "";
		senderWaitIndex = -1; //??
		return true;
	}
	return false;
}

bool Event::testAndSet() // checked by sender, turn on the flag
{
	bool tmp;
	SMARTS->contextSwitchOff( );
	tmp = flag;
	flag = true;
	SMARTS->contextSwitchOn( );
	return tmp;
}


