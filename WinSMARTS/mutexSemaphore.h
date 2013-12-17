#pragma once
#ifndef MUTEXSEMAPHORE_H
#define MUTEXSEMAPHORE_H

#include "WinSMARTS.h"
#include <queue>          // std::queue

class MutexSemaphore
{
	WinSMARTS* SMARTS;
	std::queue<tid_t> waitingList;
	bool isFree;
	tid_t owner;
	size_t level;

public:
	MutexSemaphore(WinSMARTS* SMARTS_);
	void acquire();
	void release();
};

#endif // MUTEXSEMAPHORE_H