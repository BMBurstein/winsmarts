#include "stdafx.h"
#include "timer.h"

#ifdef _WIN32

namespace 
{
	extern "C"
	{
		void popReg();
	}

	struct timerThreadStuff
	{
		HANDLE contextThread; // call the handler in the context of this thread
		TIMER_CALLBACK cb; // interupt handler
		unsigned int ms; // interval
		void* param;
		bool noStop;
	};

	// timer function to alert after "ms" miliseconds (in params)
	DWORD WINAPI timerFunc(void * param)
	{
		timerThreadStuff * tts = (timerThreadStuff*)param;
		CONTEXT ctxt;

		ctxt.ContextFlags = CONTEXT_CONTROL;

		while(tts->noStop)
		{
			Sleep(tts->ms);
			SuspendThread(tts->contextThread); // ?? alert our thread
			GetThreadContext(tts->contextThread, &ctxt);
#if defined(_X86_)
			ctxt.Esp -= sizeof(void*) * 11;
			((uintptr_t *)ctxt.Esp)[10] = ctxt.Eip;                // push return address
			((uintptr_t *)ctxt.Esp)[9] = (uintptr_t)(ctxt.EFlags); // push registers
			((uintptr_t *)ctxt.Esp)[8] = (uintptr_t)(ctxt.Eax);
			((uintptr_t *)ctxt.Esp)[7] = (uintptr_t)(ctxt.Ebx);
			((uintptr_t *)ctxt.Esp)[6] = (uintptr_t)(ctxt.Ecx);
			((uintptr_t *)ctxt.Esp)[5] = (uintptr_t)(ctxt.Edx);
			((uintptr_t *)ctxt.Esp)[4] = (uintptr_t)(ctxt.Ebp);
			((uintptr_t *)ctxt.Esp)[3] = (uintptr_t)(ctxt.Esi);
			((uintptr_t *)ctxt.Esp)[2] = (uintptr_t)(ctxt.Edi);
			((uintptr_t *)ctxt.Esp)[1] = (uintptr_t)(tts->param);  // push parameter to callback
			((uintptr_t *)ctxt.Esp)[0] = (uintptr_t)popReg;        // pop registers before returning
			ctxt.Eip = (uintptr_t)(tts->cb);
#elif defined(_AMD64_) // BROKEN!
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
} // namespace

// simulate an interrupt timer
void* setSigTimer(unsigned int ms, TIMER_CALLBACK cb, void* param)
{
	DWORD tid;
	HANDLE myHandle;
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &myHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);

	timerThreadStuff *tts = new timerThreadStuff;
	tts->contextThread = myHandle;
	tts->cb = cb; // interrupt handler
	tts->ms = ms; // interval
	tts->param = param;
	tts->noStop = true;

	HANDLE timerThread = CreateThread
		(
			NULL, // security attributes
			0, // stack size
			timerFunc, // function to be executed by the thread
			tts, // variable(s) to be passed to the thread.
			0, // creation flags
			&tid // receives the thread identifier
		);

	return &(tts->noStop);
}

void stopSigTimer(void* timer)
{
	*(bool*)timer = false;
}

#endif // _WIN32