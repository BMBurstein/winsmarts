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
		TIMER_CALLBACK interruptHandlerPointer; // interrupt handler
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
			((uintptr_t *)ctxt.Esp)[2] = (uintptr_t)ctxt.Eip;        // backup next instruction which was should be processed in our thread
			((uintptr_t *)ctxt.Esp)[1] = (uintptr_t)(tts->param);    // push parameter for interrupt handler (on our program- pointer to SMART instance)- needed at 'doTimerAsm' function
			((uintptr_t *)ctxt.Esp)[0] = (uintptr_t)(tts->interruptHandlerPointer); // push instruction pointer of interrupt handler- needed at 'doTimerAsm' function
			ctxt.Eip = (uintptr_t)(doTimerAsm);	// put pointer for 'doTimerAsm' function into instruction pointer of task which we stoped
#elif defined(_AMD64_) // BROKEN!
			ctxt.Rsp -= sizeof(void*) * 3;
			((uintptr_t *)ctxt.Rsp)[2] = (uintptr_t)ctxt.Rip;
			((uintptr_t *)ctxt.Rsp)[1] = (uintptr_t)(tts->param);
			((uintptr_t *)ctxt.Rsp)[0] = (uintptr_t)(tts->interuptHandlerPointer);
			ctxt.Rip = (uintptr_t)(doTimerAsm);
#endif
			SetThreadContext(tts->contextThread, &ctxt);	// apply the changes on registers of our thread
			ResumeThread(tts->contextThread); // resume with 'doTimerAsm' function
		}
		delete tts;

		return 0;
	}
} // namespace

// simulate an interrupt timer
void* setSigTimer(unsigned int ms, TIMER_CALLBACK interruptHandlerPointer, void* param)
{
	DWORD tid;
	HANDLE myHandle;
	// put absolute address of main thread in 'myHandle'
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &myHandle, 0, FALSE, DUPLICATE_SAME_ACCESS); 

	timerThreadStuff *tts = new timerThreadStuff;
	tts->contextThread = myHandle;	//address of main thread
	tts->interruptHandlerPointer = interruptHandlerPointer; // interrupt handler to jump to it
	tts->ms = ms; // interval	
	tts->param = param; // interrupt handler owner
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