#pragma once
#ifndef SCHEDALGO_H
#define SCHEDALGO_H

typedef int (SchedAlgo)(WinSMARTS*);

SchedAlgo RoundRobin;

#endif // SCHEDALGO_H