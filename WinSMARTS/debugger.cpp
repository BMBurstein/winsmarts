#include "debugger.h"
#include "WinSMARTS.h"
#include <stdexcept>
#include <string>
#ifdef _WIN32
 #include <Windows.h>
#endif

Debugger::Debugger(WinSMARTS* inst, unsigned short port)
	: SMARTS(inst), port(port), s(INVALID_SOCKET)
{
}

Debugger::~Debugger(void)
{
	stop();
}

void Debugger::start()
{
	DWORD tid;

	if(s != INVALID_SOCKET)
		return;

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(s == SOCKET_ERROR)
		throw std::runtime_error("Socket creation error");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(s, (sockaddr *) &addr, sizeof(addr));

#ifdef _WIN32
	HANDLE timerThread = CreateThread(
		NULL, // security attributes
		0, // stack size
		Debugger::recvLoop, // function to be executed by the thread
		this, // variable(s) to be passed to the thread.
		0, // creation flags
		&tid // receives the thread identifier
	);
#endif
}

void Debugger::stop()
{
	if(s != INVALID_SOCKET)
	{
		shutdown(s, SD_BOTH);
		closesocket(s);
		s = INVALID_SOCKET;
	}
}

DWORD WINAPI Debugger::recvLoop(void * param)
{
	sockaddr_in fromAddr;
	int fromLen = sizeof(fromAddr);
	int len;
	char msg[1000];
	char ok = '\1';
	Debugger *p = (Debugger*)param;

	while(true)
	{
		len = recvfrom(p->s, msg, 1000, 0, (sockaddr *)&fromAddr, &fromLen);
		if(len == 0 || len == SOCKET_ERROR)
			return 0;

		switch(msg[0])
		{
		case PAUSE:
			p->SMARTS->debugBegin();
			break;
		case CONTINUE:
			p->SMARTS->debugEnd();
			break;
		case STEP:
			p->SMARTS->debugStep();
			break;
		case SET_TASK:
			p->SMARTS->debugSetCurrentTask(*(int*)&msg[1]);
		}
		sendto(p->s, &ok, 1, 0, (sockaddr *)&fromAddr, fromLen);
	}
}