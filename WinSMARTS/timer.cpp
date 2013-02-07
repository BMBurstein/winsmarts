#include "timer.h"
#include "common.h"
#include <Windows.h>

#ifdef _WIN32

namespace 
{
  extern "C"
  {
    void doTimerAsm(); // implemented in assembly 
  }

  struct timerThreadStuff
  {
    HANDLE contextThread;                   // thread to interrupt
    TIMER_CALLBACK interruptHandlerPointer; // interrupt handler
    unsigned int ms;                        // interval    
    void* param;                            // parameter to handler
    bool noStop;                            // used to stop timer
  };

  // the timer loop
  DWORD WINAPI timerFunc(void * param) //??
  {
    timerThreadStuff * tts = (timerThreadStuff*)param;
    CONTEXT ctxt;

    ctxt.ContextFlags = CONTEXT_CONTROL; // we only need the SP and IP registers

    while(tts->noStop)
    {
      Sleep(tts->ms);                              // this is the "timer"
      SuspendThread(tts->contextThread);           // suspend the context thread
      GetThreadContext(tts->contextThread, &ctxt); // get registers

#if defined(_X86_)
      ctxt.Esp -= sizeof(void*) * 3;                                          // push 3 pointers
      ((uintptr_t *)ctxt.Esp)[2] = (uintptr_t)ctxt.Eip;                       // current instruction is pushed as return value from interupt
      ((uintptr_t *)ctxt.Esp)[1] = (uintptr_t)(tts->param);                   // parameter for interrupt handler
      ((uintptr_t *)ctxt.Esp)[0] = (uintptr_t)(tts->interruptHandlerPointer); // address of interrupt handler
      ctxt.Eip = (uintptr_t)(doTimerAsm);                                     // jump to 'doTimerAsm'

#elif defined(_AMD64_) // BROKEN!
      ctxt.Rsp -= sizeof(void*) * 3;
      ((uintptr_t *)ctxt.Rsp)[2] = (uintptr_t)ctxt.Rip;
      ((uintptr_t *)ctxt.Rsp)[1] = (uintptr_t)(tts->param);
      ((uintptr_t *)ctxt.Rsp)[0] = (uintptr_t)(tts->interuptHandlerPointer);
      ctxt.Rip = (uintptr_t)(doTimerAsm);
#endif

      SetThreadContext(tts->contextThread, &ctxt); // apply the changes on registers of our thread
      ResumeThread(tts->contextThread);            // resume context thread (at 'doTimerAsm')
    }
    delete tts;

    return 0;
  }
} // namespace

// simulate a timer interrupt
timerHandle setSigTimer(unsigned int ms, TIMER_CALLBACK interruptHandlerPointer, void* param)
{
  DWORD tid;
  HANDLE myHandle;
  // put absolute address of main thread in 'myHandle'
  DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &myHandle, 0, FALSE, DUPLICATE_SAME_ACCESS); 

  timerThreadStuff *tts = new timerThreadStuff;
  tts->contextThread = myHandle;  //address of main thread
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

void stopSigTimer(timerHandle &timer)
{
  *(bool*)timer = false;
  timer = 0;
}

#endif // _WIN32