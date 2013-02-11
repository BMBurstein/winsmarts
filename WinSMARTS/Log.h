#ifndef LOG_H
#define LOG_H

class Log
{
public:
  virtual ~Log() {}

  virtual void log(const char* msg, size_t len) {}
};

#endif // LOG_H