#pragma once
#include "log.h"
#include <stdio.h>
#include <windows.h>

class LogFile : public Logger
{
public:
	LogFile();
	~LogFile();

	virtual void Log(const char *type, const char *string, char*);
private:
	CRITICAL_SECTION critSect;
	FILE * fileHandle;
};
