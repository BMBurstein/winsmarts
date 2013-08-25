#pragma once
#ifndef LOGFILE_H
#define LOGFILE_H

#include "log.h"
#include <fstream>
#include <string>

class LogFile : public Log
{
	std::ofstream logFile;
	std::string fileName;

public:
	LogFile(char const* file);
	virtual ~LogFile(void);
	virtual void log(const char* msg, size_t len);
	virtual void clear();
};

#endif