#pragma once
#ifndef COUNTINGSEMAPHORE_H
#define COUNTINGSEMAPHORE_H

#include "WinSMARTS.h"
#include <queue>          // std::queue
#include <string>

class CountingSemaphore
{
	WinSMARTS* SMARTS;
	std::queue<tid_t> waitingList;
	int maxAuthorized;
	int free;
	std::string name;

	static unsigned long long semIdCounter;

public:	
	CountingSemaphore (WinSMARTS* SMARTS, int authorized = 0);
	CountingSemaphore(WinSMARTS* SMARTS, std::string name, int authorized = 0);
	void acquire();
	void release();
};

#endif // COUNTINGSEMAPHORE_H