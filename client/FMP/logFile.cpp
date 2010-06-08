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
	DeleteCriticalSection(&critSect);
}

void LogFile::Log(const wchar_t *type, const wchar_t *string, char *arglist)
{
	if(!fileHandle) return;
	EnterCriticalSection(&critSect);
	if(!fileHandle) return;

	SYSTEMTIME time;
	GetLocalTime(&time);
	if(wcslen(type) > 0)
		fwprintf(fileHandle, L"[%02d:%02d:%02d:%03d] %s: ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, type);
	else
		fwprintf(fileHandle, L"[%02d:%02d:%02d:%03d]: ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	vfwprintf(fileHandle, string, arglist);
	fwprintf(fileHandle, L"\n");
	fflush(fileHandle);

	LeaveCriticalSection(&critSect);
}