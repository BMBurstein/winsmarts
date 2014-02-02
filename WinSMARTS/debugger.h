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

private:
	WinSMARTS* SMARTS;
	SOCKET s;
	SOCKADDR_IN addr;
	WinSock winSock;
	unsigned short port;

	static DWORD WINAPI recvLoop(void *);

	enum DEBUG_COMMANDS
	{
		PAUSE,
		CONTINUE,
		GET_ALL,
		STEP,
		SET_TASK,
		SET_CS,
	};
};

#endif // DEBUGGER_H