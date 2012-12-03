#include "stdafx.h"
#include "timer.h"

#ifdef _WIN32

namespace {
	struct timerThreadStuff
	{
		HANDLE contextThread;
		TIMER_CALLBACK cb;
		unsigned int ms;
	};

	static DWORD WINAPI timerFunc(void * param)
	{
		timerThreadStuff * tts = (timerThreadStuff*)param;
		CONTEXT ctxt;

		ctxt.ContextFlags = CONTEXT_CONTROL;

		while(1)
		{
			Sleep(tts->ms);
			SuspendThread(tts->contextThread);
			GetThreadContext(tts->contextThread, &ctxt);
#if defined(_X86_)
			ctxt.Esp -= sizeof(ctxt.Eip);
			*((DWORD *)ctxt.Esp) = ctxt.Eip;
			ctxt.Eip = (uintptr_t)(tts->cb);
#elif defined(_AMD64_)
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

void setSigTimer(unsigned int ms, TIMER_CALLBACK cb)
{
	DWORD tid;
	HANDLE myHandle;
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &myHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
	timerThreadStuff *tts = new timerThreadStuff;
	tts->contextThread = myHandle;
	tts->cb = cb;
	tts->ms = ms;
	HANDLE timerThread = CreateThread(NULL, 0, timerFunc, tts, 0, &tid);
}

#endif // _WIN32