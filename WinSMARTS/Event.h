#pragma once  //??
#ifndef EVENT_H  //??
#define EVENT_H  //??

#include <string>
class WinSMARTS;

//using namespace std;

class Event
{
private:
	WinSMARTS* SMARTS; 

	void *data;      // the parameter
	bool  isEventWaitForReceiver;   // arrived flag
	std::string source;     // source task name
	std::string dest;       // destination task name
	int senderWaitIndex;  // tells if the sender has to wait till the reciver receives
	bool testAndSet();// test and set 'flag' in an 'atomic transaction'

public:
	Event(WinSMARTS* SMARTSp);
	// sends an event to 'dest', if 'synch' is true waits until the receiver gets it.
	void send(std::string targetName, void *param, bool synch);
	// waits for the event, and returns the parameter. 
	void* wait(std::string &sourceP); //'sourceP' will contain the task sender ID.
	// check for for an arrived event. 'sourceP' will contain the task sender ID.
	bool isArrivedEvent(std::string &sourceP);
	// resets the event
	bool reset();
};

#endif // EVENT_H