#pragma once
#ifndef LOGUDP_H
#define LOGUDP_H

#include "log.h"
#include "WinSock.h"

class LogUDP : public Log
{
  SOCKET s;
  SOCKADDR_IN addr;

public:
  LogUDP(u_short port = 44557, char const* ip = "127.0.0.1");
  virtual ~LogUDP(void);
  virtual void log(const char* msg, size_t len);
  virtual void clear();
};

#endif