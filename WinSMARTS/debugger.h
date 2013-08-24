#pragma once
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "WinSock.h"

class WinSMARTS;

class Debugger
{
public:
	Debugger(WinSMARTS* inst, unsigned short port=44558);
	~Debugger();

	void start();
	void stop();

	void doPause();
	void doContinue();

private:
	WinSMARTS* SMARTS;
	SOCKET s;
	SOCKADDR_IN addr;
	WinSock winSock;
	unsigned short port;

	enum DEBUG_COMMANDS
	{
		PAUSE,
		CONTINUE,
	};
};

#endif // DEBUGGER_H