#include "logUDP.h"
#include <stdexcept>

LogUDP::LogUDP(u_short port, char const* ip)
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

void LogUDP::log(const char* msg, size_t len)
{
  sendto(s, msg, len, 0, (sockaddr *) &addr, sizeof(addr));
}

void LogUDP::clear()
{
  log("", 0);
}