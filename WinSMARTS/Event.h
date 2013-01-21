#pragma once	//??
#ifndef EVENT_H	//??
#define EVENT_H	//??

#include <string>
class WinSMARTS;

//using namespace std;

class Event
{
private:
	WinSMARTS* SMARTS; 

	void *data;      // the parameter
	bool  flag;   // arrived flag
	std::string source;     // source task name
	std::string dest;       // destination task name
	bool senderWaitIndex;  // tells if the sender has to wait till the reciver receives
	bool testAndSet();// test and set 'flag' in an 'atomic transaction'

 public:
	Event(WinSMARTS* SMARTSp);
	// sends an event to 'dest', if 'synch' is true waits until the receiver gets it.
	void send(std::string dest,void *param, int synch);
	// waits for the event, and returns the parameter. 
	void* wait(std::string &sourceP); //'sourceP' will contain the task sender ID.
	// check for for an arrived event. 'sourceP' will contain the task sender ID.
	bool isArrived(std::string &sourceP);
	// resets the event
	bool reset();
};

#endif // EVENT_H