#pragma once
#ifndef SCHEDALGO_H
#define SCHEDALGO_H

#include <set>
#include <map>
#include <algorithm>
#include "WinSMARTS.h"

typedef size_t (SchedAlgo)(std::set<tid_t> states[NUM_OF_STATUSES], tid_t current, WinSMARTS* SMARTS);

SchedAlgo RoundRobin;
SchedAlgo ByPriority;
SchedAlgo EDF;

#endif // SCHEDALGO_H