#include "StdAfx.h"
#include "WinSMARTS.h"
#include "timer.h"
#include "schedAlgo.h"

using namespace std;

WinSMARTS::WinSMARTS()
	: timerInterval(55)
{
}

WinSMARTS::~WinSMARTS()
{
}


void static timerHandler()
{
	cout << "Time!" << endl;
}

void WinSMARTS::runTheTasks()
{
	setSigTimer(timerInterval, timerHandler);
}

static void taskEnd(void* param)
{
	((WinSMARTS*)param)->taskEnd();
}

void WinSMARTS::declareTask(taskProc fn, std::string const &name, int priority)
{
	//context.emplace_back(fn, name, priority, ::taskEnd);
	tasks.push_back(Task(fn, name, priority, ::taskEnd, this));
}

void WinSMARTS::taskEnd()
{
}