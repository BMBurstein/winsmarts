#include "Event.h"
#include "WinSMARTS.h"

Event::Event(WinSMARTS* SMARTSp)
{
  SMARTS = SMARTSp;
  reset();
}

void Event::send(std::string targetName, void *param, bool synch)
{
  while(testAndSet( )); // while the receiver not read previous data yet

  this->source = SMARTS->getCurrentName( );
  this->data = param;

  int i;
  for (i=1; i < SMARTS->getTotalTasks(); i++)
    if (SMARTS->getName(i) == targetName)
      break;
  if (i < SMARTS->getTotalTasks()) // if target found
  {
    if (SMARTS->getStatus(i)==SUSPENDED && SMARTS->getExpectedEvent(i)==this) // if the target is suspended and it wait for this event
      SMARTS->setStatus(i,READY);

    if (synch)
    {
      senderWaitIndex = SMARTS->getCurrentTask( );
      SMARTS->setCurrentStatus(SUSPENDED);
      dest = targetName;
      SMARTS->callScheduler( );
    }
    else
      dest = targetName;
  }
}

void *Event::wait(std::string &sourceP)
{
  void* param;                                    //delivered data
  if (!isEventWaitForReceiver || dest!=SMARTS->getCurrentName())    // if nothing received yet
  {
    SMARTS->setCurrentExpectedEvent(this);
    SMARTS->setCurrentStatus(SUSPENDED);
    SMARTS->callScheduler();
  }

  sourceP = source;
  param = data;
  if (senderWaitIndex >= 0)                        // if synchronic sending
    SMARTS->setStatus(senderWaitIndex,READY);
  reset();
  return (param);
}

bool Event::isArrivedEvent(std::string &sourceP) // checked by target, if so get sender name
{
  if (isEventWaitForReceiver)
    sourceP = source;
  return isEventWaitForReceiver;
}

bool Event::reset()
{
  if (isEventWaitForReceiver) 
  {
    isEventWaitForReceiver=false;
    data = NULL;
    source = dest = "";
    senderWaitIndex = -1;
    return true;
  }
  return false;
}

bool Event::testAndSet() // checked by sender, turn on the isEventWaitForReceiver
{
  bool tmp;
  SMARTS->contextSwitchOff( );
  tmp = isEventWaitForReceiver;
  isEventWaitForReceiver = true;
  SMARTS->contextSwitchOn( );
  if (tmp) SMARTS->callScheduler(); // giving up CPU time
  return tmp;
}


