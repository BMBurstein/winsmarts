#pragma once
#ifndef TIMER_H
#define TIMER_H

typedef void (__stdcall * TIMER_CALLBACK)(void*);

// create thread for timer
void setSigTimer(unsigned int ms, TIMER_CALLBACK cb, void* param);

#endif // TIMER_H