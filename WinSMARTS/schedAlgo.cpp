#include "WinSMARTS.h"

tid_t RoundRobin(std::set<tid_t> states[NUM_OF_STATUSES], tid_t current, WinSMARTS* SMARTS)
{
	if (states[READY].size() <= 1)
		return *states[READY].begin();

	std::set<tid_t>::iterator nextTask = states[READY].upper_bound(current);
	if(nextTask == states[READY].end())
		nextTask = ++states[READY].begin();

	return *nextTask;
}

tid_t ByPriority(std::set<tid_t> states[NUM_OF_STATUSES], tid_t current, WinSMARTS* SMARTS)
{
	if (states[READY].size() <= 1)
		return *states[READY].begin();
	
	std::multimap<int, tid_t> TasksByPriority;
	for (std::set<tid_t>::iterator it = states[READY].begin(); it != states[READY].end(); it++)
	{
		TasksByPriority.insert(std::pair<int, tid_t> (SMARTS->getTaskPriority(*it),*it));
	}

	std::vector<tid_t> HighPriorityTasks;
	int MaxPriority = TasksByPriority.rbegin()->first;
	for (std::multimap<int, tid_t>::reverse_iterator it = TasksByPriority.rbegin(); it->first == MaxPriority; it++)
	{
		HighPriorityTasks.push_back(it->second);
	}

	std::vector<tid_t>::iterator nextTask = std::find(HighPriorityTasks.begin(), HighPriorityTasks.end(), current);
	if (nextTask == HighPriorityTasks.end())
		return *HighPriorityTasks.begin();

	if(++nextTask == HighPriorityTasks.end())
		nextTask = HighPriorityTasks.begin();

	return *nextTask;
}




tid_t EDF(std::set<tid_t> states[NUM_OF_STATUSES], tid_t current, WinSMARTS* SMARTS)
{
	unsigned int minLeftTime = -1;
	tid_t minLeftTimeTask = *states[READY].begin();

	for (std::set<tid_t>::iterator it = states[READY].begin(); it != states[READY].end(); it++)
	{
		if (SMARTS->getTaskLeftCyclePeriod(*it) < minLeftTime)
		{
			minLeftTime = SMARTS->getTaskLeftCyclePeriod(*it);
			minLeftTimeTask = *it;
		}
	}

	return minLeftTimeTask;	
}