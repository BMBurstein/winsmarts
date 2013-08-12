#ifdef _WIN32

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

class WinSock
{
public:
	WinSock()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2,2), &wsaData);
	}
	~WinSock()
	{
		WSACleanup();
	}
};

static WinSock unusedWinSockVar;

#endif //_WIN32