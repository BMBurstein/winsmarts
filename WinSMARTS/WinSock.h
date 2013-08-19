#pragma once
#ifndef WINSOCK_H
#define WINSOCK_H

#ifdef _WIN32

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

class WinSock
{
public:
	WinSock()
	{
		WSADATA wsaData;
		if(!(inst++))
			WSAStartup(MAKEWORD(2,2), &wsaData);
	}
	~WinSock()
	{
		if(!--inst)
			WSACleanup();
	}
private:
	static int inst;
};

static WinSock unusedWinSockVar;

#endif //_WIN32

#endif // WINSOCK_H