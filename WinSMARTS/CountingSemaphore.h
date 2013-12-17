#pragma once
#ifndef COUNTINGSEMAPHORE_H
#define COUNTINGSEMAPHORE_H

#include "WinSMARTS.h"
#include <queue>          // std::queue

class CountingSemaphore
{
	WinSMARTS* SMARTS;
	std::queue<tid_t> waitingList;
	int maxAuthorized;
	int free;
	unsigned int semId;

	static unsigned int semIdCounter;


public:	
	CountingSemaphore (WinSMARTS* SMARTS_, int authorized = 0);
	void acquire();
	void release();
};

#endif // COUNTINGSEMAPHORE_H