#pragma once
#ifndef DEBUGGER_H
#define DEBUGGER_H

class WinSMARTS;

class Debugger
{
public:
	Debugger(WinSMARTS* inst, unsigned short port=44558);

private:
	WinSMARTS* SMARTS;
	unsigned short port;
};

#endif // DEBUGGER_H