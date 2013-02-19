#include "logUDP.h"
#include <stdexcept>

LogUDP::LogUDP(u_short port, char const* ip)
{
#ifdef _WIN32
  WSADATA wsaData;
  if(WSAStartup(MAKEWORD(2,2), &wsaData))
    throw std::runtime_error("Could not init Winsock");
#endif //_WIN32

  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(s == SOCKET_ERROR)
    throw std::runtime_error("Socket creation error");

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);
}

LogUDP::~LogUDP(void)
{
  closesocket(s);
#ifdef _WIN32
  WSACleanup();
#endif
}

void LogUDP::log(const char* msg, size_t len)
{
  sendto(s, msg, len, 0, (struct sockaddr *) &addr, sizeof(addr));
}

void LogUDP::clear()
{
  log("clear", 6);
}