#pragma once
#ifndef LOG_H
#define LOG_H

class Log
{
public:
  virtual ~Log() {}

  virtual void log(const char* msg, size_t len) {}
  virtual void clear();
};

#endif // LOG_H