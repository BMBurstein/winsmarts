#include "debugger.h"
#include "WinSMARTS.h"
#include <stdexcept>
#include <string>

Debugger::Debugger(WinSMARTS* inst, unsigned short port)
    : SMARTS(inst), port(port)
{
}

Debugger::~Debugger(void)
{
  stop();
}

void Debugger::start()
{
  sockaddr_in fromAddr;
  int fromLen = sizeof(fromAddr);
  int len;
  char msg[1000];
  int ret;

  if(s != INVALID_SOCKET)
    return;

  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(s == SOCKET_ERROR)
    throw std::runtime_error("Socket creation error");

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s, (sockaddr *) &addr, sizeof(addr));

  while(true)
  {
      len = recvfrom(s, msg, 1000, 0, (sockaddr *)&fromAddr, &fromLen);
      if(len == 0)
          return;
      if(len == SOCKET_ERROR)
        throw std::runtime_error("Receve error: ");

      switch(msg[0])
      {
          case PAUSE:
              doPause();
              break;
          case CONTINUE:
              doContinue();
              break;
      }
  }
  sendto(s, msg, len, 0, (sockaddr *) &addr, sizeof(addr));
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

void Debugger::doPause()
{
    SMARTS->debugBegin();
}

void Debugger::doContinue()
{
    SMARTS->debugEnd();
}