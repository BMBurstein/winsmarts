#pragma once
#ifndef LOGFILE_H
#define LOGFILE_H

#include "log.h"
#include <fstream>

class LogFile : public Log
{
  std::ofstream logFile;

public:
  LogFile(char const* file);
  virtual ~LogFile(void);
  virtual void log(const char* msg, size_t len);
};

#endif