#include "stdafx.h"
#include "timer.h"

#ifdef _WIN32

namespace 
{
	struct timerThreadStuff
	{
		HANDLE contextThread; // the thread should be alert
		TIMER_CALLBACK cb; // target function
		unsigned int ms; // interval
		void* param; //needed variables
	};

	//timer function to alert after "ms" miliseconds (in params)
	DWORD WINAPI timerFunc(void * param)
	{
		timerThreadStuff * tts = (timerThreadStuff*)param;
		CONTEXT ctxt;

		ctxt.ContextFlags = CONTEXT_CONTROL;

		while(1)
		{
			Sleep(tts->ms);
			SuspendThread(tts->contextThread); // ?? alert our thread
			GetThreadContext(tts->contextThread, &ctxt);
#if defined(_X86_)
			ctxt.Esp -= sizeof(tts->param);
			*((uintptr_t *)ctxt.Esp) = (uintptr_t)tts->param;
			ctxt.Esp -= sizeof(ctxt.Eip);
			*((DWORD *)ctxt.Esp) = ctxt.Eip;
			ctxt.Eip = (uintptr_t)(tts->cb);
#elif defined(_AMD64_)
			ctxt.Rsp -= sizeof(tts->param);
			*((uintptr_t *)ctxt.Rsp) = (uintptr_t)tts->param;
			ctxt.Rsp -= sizeof(ctxt.Rip);
			*((DWORD64 *)ctxt.Rsp) = ctxt.Rip;
			ctxt.Rip = (uintptr_t)(tts->cb);
#endif
			SetThreadContext(tts->contextThread, &ctxt);
			ResumeThread(tts->contextThread);
		}
		delete tts;

		return 0;
	}
}

// create thread for timer
void setSigTimer(unsigned int ms, TIMER_CALLBACK cb, void* param)
{
	DWORD tid; // 4 bites allocation
	HANDLE myHandle; //  void* pointer
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &myHandle, 0, FALSE, DUPLICATE_SAME_ACCESS); //??

	timerThreadStuff *tts = new timerThreadStuff;
	tts->contextThread = myHandle; // ?? where are we initialize this param?
	tts->cb = cb; // target function
	tts->ms = ms; // interval
	tts->param = param; //needed variables

	HANDLE timerThread = CreateThread
		(
			NULL, // security attributes
			0, // stack size
			timerFunc, // function to be executed by the thread
			tts, // variable(s) to be passed to the thread.
			0, // creation flags
			&tid // receives the thread identifier
		);
}

#endif // _WIN32