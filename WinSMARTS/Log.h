#ifndef LOG_H
#define LOG_H

class Log
{
public:
  virtual ~Log() {}

  virtual void log(const char*) {};
};

#endif // LOG_H