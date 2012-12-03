/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory                 */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum                  */
/* Jerusalem College of Technology, 5759-64 (1999)                            */
/* SchedAlgo64.CPP, short-term scheduler algorithms                */
/****************************************************************/


#include "smarts64.h"

int RoundRobin( )
// Round Robin short-term algorithm 
{
  int count;
  int nextTask = SMARTS.getCurrentTask()+1;
	if (nextTask >= SMARTS.getTotalTask( ))
		nextTask = 0;
	for (count=0;
	     SMARTS.getStatus(nextTask)!=ready && count<SMARTS.getTotalTask( );
	     count++)
	     nextTask=++nextTask%SMARTS.getTotalTask( );
	if (count == SMARTS.getTotalTask( ))	// no found ready task to run
		nextTask = SMARTS.getTotalTask( );
	return nextTask;
}


