#pragma once
#ifndef SCHEDALGO_H
#define SCHEDALGO_H

typedef size_t (SchedAlgo)(WinSMARTS*);

SchedAlgo RoundRobin;

#endif // SCHEDALGO_H