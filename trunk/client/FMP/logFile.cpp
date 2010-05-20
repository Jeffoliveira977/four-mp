#include "logFile.h"

LogFile::LogFile()
{
	InitializeCriticalSection(&critSect);
	fopen_s(&fileHandle, "FMP\\log.txt", "a");
	if(!fileHandle) exit(1);
}

LogFile::~LogFile()
{
	if(fileHandle) fclose(fileHandle);
}

void LogFile::Log(const char *type, const char *string, char *arglist)
{
	if(!fileHandle) return;
	EnterCriticalSection(&critSect);
	if(!fileHandle) return;

	SYSTEMTIME time;
	GetLocalTime(&time);
	if(strlen(type) > 0)
		fprintf(fileHandle, "[%02d:%02d:%02d:%03d] %s: ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, type);
	else
		fprintf(fileHandle, "[%02d:%02d:%02d:%03d]: ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	vfprintf(fileHandle, string, arglist);
	fprintf(fileHandle, "\n");
	fflush(fileHandle);

	LeaveCriticalSection(&critSect);
}

