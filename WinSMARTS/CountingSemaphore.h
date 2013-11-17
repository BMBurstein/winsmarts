#pragma once
#ifndef COUNTINGSEMAPHORE_H
#define COUNTINGSEMAPHORE_H

#include "WinSMARTS.h"
#include <queue>          // std::queue

class CountingSemaphore
{
	std::queue<tid_t> waitingList;
	WinSMARTS* SMARTS;
	int maxAuthorized;
	int free;


public:	
	CountingSemaphore (WinSMARTS* SMARTS_, int authorized = 0);
	void acquire();
	void release();
};

#endif // COUNTINGSEMAPHORE_H