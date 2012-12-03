#pragma once
#ifndef TIMER_H
#define TIMER_H

typedef void (*TIMER_CALLBACK)();

void setSigTimer(unsigned int ms, TIMER_CALLBACK cb);

#endif // TIMER_H