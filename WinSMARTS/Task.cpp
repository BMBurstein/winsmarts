#include "Task.h"


Task::Task(TaskProc fn, std::string const &name_, int priority_, TaskProc taskEnd, WinSMARTS* smarts)
  : name          (name_),
    priority      (priority_),
    origPriority  (priority_),
    status        (READY),
    expectedEvent (NULL),
    CSOff         (false)
{
  taskPtr = newTask(fn, smarts, stack + 65536, taskEnd, smarts);    //initialize the stack of the new task and save stack pointer
}

void Task::sleepDecr()
{
  if(status == SLEEPING)
  {
    if(sleepCounter > 0)
      --sleepCounter;
    if(sleepCounter == 0)
      status = READY;
  }
}
