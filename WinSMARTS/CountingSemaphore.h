#pragma once
#ifndef COUNTINGSEMAPHORE_H
#define COUNTINGSEMAPHORE_H

#include "WinSMARTS.h"
#include <queue>          // std::queue

class countingSemaphore
{
	WinSMARTS* SMARTS;
	std::queue<tid_t> waitingList;
	int maxAuthorized;
	int free;


public:	
	countingSemaphore (WinSMARTS* SMARTS_, int authorized = 0);
	void acquire();
	void release();
};

#endif // COUNTINGSEMAPHORE_H