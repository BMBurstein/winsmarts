#include "WinSMARTS.h"

tid_t RoundRobin(std::set<tid_t> states[NUM_OF_STATUSES], tid_t current)
{
	if (states[READY].size() <= 1)
		return *states[READY].begin();

	std::set<tid_t>::iterator nextTask = states[READY].upper_bound(current);
	if(nextTask == states[READY].end())
		nextTask = ++states[READY].begin();

	return *nextTask;
}