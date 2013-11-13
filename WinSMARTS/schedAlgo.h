#pragma once
#ifndef SCHEDALGO_H
#define SCHEDALGO_H

#include <set>

typedef size_t (SchedAlgo)(std::set<tid_t> states[NUM_OF_STATUSES], tid_t current);

SchedAlgo RoundRobin;

#endif // SCHEDALGO_H