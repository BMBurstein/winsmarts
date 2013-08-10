#include <iostream>
#include "LogUDP.h"
#include "LogFile.h"
#include "WinSMARTS.h"
using namespace std;

void BusyWait(long long j)
{
  for(volatile long long i=0; i<j; i++);
}

void __stdcall a(WinSMARTS * SMARTS)
{
  for(int i=0; i<50; ++i)
  {
    SMARTS->contextSwitchOff();
    cout << 'a';
    SMARTS->contextSwitchOn();
    BusyWait(400000);
  }
}

void __stdcall b(WinSMARTS * SMARTS)
{
  for(int i=0; i<50; ++i)
  {
    SMARTS->contextSwitchOff();
    cout << 'b';
    SMARTS->contextSwitchOn();
    BusyWait(4000000);
  }
}

void __stdcall c(WinSMARTS * SMARTS)
{
  for(int i=0; i<50; ++i)
  {
    SMARTS->contextSwitchOff();
    cout << 'c';
    SMARTS->contextSwitchOn();
    BusyWait(40000000);
  }
}




/********************************************************/
LogUDP Lg;
//LogFile Lg("Output.txt");
WinSMARTS SMARTS(RoundRobin, Lg, 55);
Event e(&SMARTS);
void __stdcall D(WinSMARTS * SMARTS)
{
  SMARTS->contextSwitchOff();
  cout << 'D';
  SMARTS->contextSwitchOn();

  e.wait((string)"E");

  for(int i=0; i<30; ++i) BusyWait(40000000);
  //for(int i=0; i<30; ++i) {BusyWait(20000000); if (i==15) SMARTS->setTaskStatus(SUSPENDED);}

  SMARTS->contextSwitchOff();
  cout << 'D';
  SMARTS->contextSwitchOn();
}

void __stdcall E(WinSMARTS * SMARTS)
{

  SMARTS->contextSwitchOff();
  cout << 'E';
  SMARTS->contextSwitchOn();

  for(int i=0; i<30; ++i) BusyWait(20000000);
  for(int i=0; i<30; ++i) {BusyWait(20000000); if (i==15) SMARTS->setTaskStatus(SUSPENDED);}

  SMARTS->contextSwitchOff();
  cout << 'E';
  SMARTS->contextSwitchOn();
  e.send("D",NULL,false);
}
/********************************************************/

int main()
{
  setvbuf(stdout, NULL, _IONBF, 0);  // cancel buffering on screen printing

  //WinSMARTS SMARTS(RoundRobin, Lg, 55);      // instance of our system
  

  SMARTS.declareTask(a, "a", 5);    //
  SMARTS.declareTask(b, "b", 5);    // declare few tasks
  SMARTS.declareTask(c, "c", 5);    //

  SMARTS.declareTask(D, "D", 5);
  SMARTS.declareTask(E, "E", 5);
    
  SMARTS.runTheTasks();        // start running the tasks

  return 0;
}