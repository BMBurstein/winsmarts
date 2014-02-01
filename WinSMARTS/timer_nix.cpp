#include "platform.h"
#include "timer.h"

#include <signal.h>
#include <sys/time.h>

namespace 
{
	void* sigTimerParam_;
	TIMER_CALLBACK sigTimerFunc_;
	unsigned int uS_;

	void sigTimerCB_(int sig)
	{
		signal (sig, sigTimerCB_);
		sigTimerFunc_(sigTimerParam_);
	}

	void sigTimerStart_()
	{
		itimerval newT;
		newT.it_interval.tv_usec = uS_;
		newT.it_interval.tv_sec = 0;
		newT.it_value.tv_usec = uS_;
		newT.it_value.tv_sec = 0;
		setitimer(ITIMER_REAL, &newT, NULL);
	}

	void sigTimerStop_()
	{
		itimerval newT;
		newT.it_interval.tv_usec = 0;
		newT.it_interval.tv_sec = 0;
		newT.it_value.tv_usec = 0;
		newT.it_value.tv_sec = 0;
		setitimer(ITIMER_REAL, &newT, NULL);
	}
} // namespace

// simulate a timer interrupt
timerHandle setSigTimer(unsigned int ms, TIMER_CALLBACK interruptHandlerPointer, void* param)
{
	sigTimerFunc_ = interruptHandlerPointer;
	sigTimetParam_ = param;
	uS_ = 1000 * ms;

	signal (SIGALRM, sigTimerCB_);

	sigTimerStart_();

	return NULL;
}

void stopSigTimer(timerHandle &timer)
{
	sigTimerStop_();
}

void pauseSigTimer(timerHandle &timer)
{
	sigTimerStop_();
}

void resumeSigTimer(timerHandle &timer)
{
	sigTimerStart_();
}