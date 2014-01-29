#pragma once
#ifndef MUTEXSEMAPHORE_H
#define MUTEXSEMAPHORE_H

#include "WinSMARTS.h"
#include <queue>          // std::queue
#include <string>

class MutexSemaphore
{
	WinSMARTS* SMARTS;
	std::queue<tid_t> waitingList;
	bool isFree;
	tid_t owner;
	size_t level;
	std::string name;

	static unsigned long long semIdCounter;

public:
	MutexSemaphore(WinSMARTS* SMARTS);
	MutexSemaphore(WinSMARTS* SMARTS, std::string name);
	void acquire();
	void release();
};

#endif // MUTEXSEMAPHORE_H