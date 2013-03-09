#pragma once
#ifndef TIMER_H
#define TIMER_H

typedef void (__stdcall * TIMER_CALLBACK)(void*); // "timer interrupt" handler
typedef void* timerHandle;

// simulate timer interrupt (call 'cb(param)' every 'ms' milliseconds)
// returns a handle to created timer
timerHandle setSigTimer(unsigned int ms, TIMER_CALLBACK cb, void* param);

//cancels the timer interrupt
void stopSigTimer(timerHandle &timer);
//suspends interrupts from the timer
void pauseSigTimer(timerHandle &timer);
//resumes interrupts from the timer
void resumeSigTimer(timerHandle &timer);

#endif // TIMER_H