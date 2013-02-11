#pragma once
#pragma comment(lib, "Ws2_32.lib")
#ifndef LOGUDP_H
#define LOGUDP_H

#include "log.h"
#include <stdexcept>
#include <winsock2.h>


class LogUDP : public Log
{
  SOCKET s;
  SOCKADDR_IN addr;
public:
  LogUDP(char* ip = "127.0.0.1", u_short port = 44557)
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

  virtual ~LogUDP(void)
  {
    closesocket(s);
#ifdef _WIN32
    WSACleanup();
#endif
  }

  virtual void log(const char* msg, size_t len)
  {
    sendto(s, msg, len, 0, (struct sockaddr *) &addr, sizeof(addr));
  }
};

#endif