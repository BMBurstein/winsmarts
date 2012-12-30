#include "timer.h"
#include "common.h"
#include <Windows.h>

#ifdef _WIN32

namespace 
{
	extern "C"
	{
		void doTimerAsm();
	}

	struct timerThreadStuff
	{
		HANDLE contextThread; // call the handler in the context of this thread
		TIMER_CALLBACK interuptHandlerPointer; // interupt handler
		unsigned int ms; // interval		
		void* param;
		bool noStop;
	};

	// timer function to alert after "ms" miliseconds (in params)
	DWORD WINAPI timerFunc(void * param)
	{
		timerThreadStuff * tts = (timerThreadStuff*)param;
		CONTEXT ctxt;

		ctxt.ContextFlags = CONTEXT_CONTROL; //??

		while(tts->noStop)
		{
			Sleep(tts->ms);
			SuspendThread(tts->contextThread); // stop our thread
			GetThreadContext(tts->contextThread, &ctxt);
#if defined(_X86_)
			ctxt.Esp -= sizeof(void*) * 3;
			((uintptr_t *)ctxt.Esp)[2] = (uintptr_t)ctxt.Eip;        // pop registers before returning
			((uintptr_t *)ctxt.Esp)[1] = (uintptr_t)(tts->param);  // push parameter to callback
			((uintptr_t *)ctxt.Esp)[0] = (uintptr_t)(tts->interuptHandlerPointer);
			ctxt.Eip = (uintptr_t)(doTimerAsm);
#elif defined(_AMD64_) // BROKEN!
			ctxt.Rsp -= sizeof(tts->param);
			*((uintptr_t *)ctxt.Rsp) = (uintptr_t)tts->param;
			ctxt.Rsp -= sizeof(ctxt.Rip);
			*((DWORD64 *)ctxt.Rsp) = ctxt.Rip;
			ctxt.Rip = (uintptr_t)(tts->interuptHandlerPointer);
#endif
			SetThreadContext(tts->contextThread, &ctxt);
			ResumeThread(tts->contextThread);
		}
		delete tts;

		return 0;
	}
} // namespace

// simulate an interrupt timer
void* setSigTimer(unsigned int ms, TIMER_CALLBACK interuptHandlerPointer, void* param)
{
	DWORD tid;
	HANDLE myHandle;
	// put absolute address of main thread in 'myHandle'
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &myHandle, 0, FALSE, DUPLICATE_SAME_ACCESS); 

	timerThreadStuff *tts = new timerThreadStuff;
	tts->contextThread = myHandle;	//address of main thread
	tts->interuptHandlerPointer = interuptHandlerPointer; // interrupt handler to jump to it
	tts->ms = ms; // interval	
	tts->param = param; // ?? why we need it?
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