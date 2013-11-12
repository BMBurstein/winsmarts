#include "logUDP.h"
#include <stdexcept>

LogUDP::LogUDP(u_short port, char const* ip)
	: count(0)
{
	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(s == SOCKET_ERROR)
		throw std::runtime_error("Socket creation error");

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
}

LogUDP::~LogUDP(void)
{
	closesocket(s);
}

void LogUDP::log(LogMsg evt, const char* msg, size_t len)
{
	static char buff[260];

	int c = htonl(++count);
	buff[0] = (char)evt;
	memcpy(&buff[1], &c, 4);
	memcpy(&buff[5], msg, len);

	sendto(s, buff, (int)len + 5, 0, (sockaddr *) &addr, sizeof(addr));
}

void LogUDP::clear()
{
	log(LOG_START, NULL, 0);
}