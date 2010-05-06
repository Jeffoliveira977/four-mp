/*
*	Created:			29.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include <io.h>
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

#include "log.h"
#include "ConsoleWindow.h"

extern ConsoleWindow conwindow;
CRITICAL_SECTION cs_log, cs_debug;
FILE *f_log, *f_debug;

void InitLogs()
{
	debug_clear();
	log_clear();

#ifdef DEBUG_ON
	InitializeCriticalSection(&cs_debug);
	f_debug = fopen("FMP\\debug.txt", "a");
#endif

#ifdef LOGGING_ON
	InitializeCriticalSection(&cs_log);
	f_log = fopen("FMP\\log.txt", "a");
#endif
}

void CloseLogs()
{
	if(f_log) fclose(f_log);
	if(f_debug) fclose(f_debug);
}

bool FileExists(const char *fname)
{
	return _access(fname, 0) != -1;
}

void PrintToConsole(const char *string, ...)
{
#ifdef PRINT_TO_CONSOLE
	va_list arglist; 
    va_start(arglist, string);
	char *tempstring = (char *)calloc(_vscprintf(string, arglist) + 1, sizeof(char));
	vsprintf(tempstring, string, arglist); 
	conwindow.Print(tempstring);
	free(tempstring);
	va_end(arglist); 
#endif
}

void debug(const char* string)
{
	#if defined DEBUG_ON
	if(!f_debug) return;
	EnterCriticalSection(&cs_debug);
	if(!f_debug) return;

#ifdef PRINT_TO_CONSOLE
	conwindow.Print(string);
#endif

	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(f_debug, "[%02d:%02d:%02d:%02d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	fputs(string,f_debug);
	fputs("\n",f_debug);
	fflush(f_debug);
	
	LeaveCriticalSection(&cs_debug);
	#endif
}

void debug_clear()
{
	if(FileExists("FMP\\debug.txt")) DeleteFile("FMP\\debug.txt");
}

void log(const char* string)
{
	#if defined LOGGING_ON
	if(!f_log) return;
	EnterCriticalSection(&cs_log);
	if(!f_log) return;

#ifdef PRINT_TO_CONSOLE
	conwindow.Print(string);
#endif

	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(f_log, "[%02d:%02d:%02d:%02d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	fputs(string,f_log);
	fputs("\n",f_log);
	fflush(f_log);
	
	LeaveCriticalSection(&cs_log);
	#endif
}

void log_clear()
{
	if(FileExists("FMP\\log.txt")) DeleteFile("FMP\\log.txt");
}

void Debug(const char *string, ...)
{
	#if defined DEBUG_ON
	if(!f_debug) return;
	EnterCriticalSection(&cs_debug);
	if(!f_debug) return;

	char buff[512];
	va_list arglist; 
    va_start(arglist, string); 
	vsprintf_s(buff,512,string,arglist);

#ifdef PRINT_TO_CONSOLE
	conwindow.Print(string);
#endif

	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(f_debug, "[%02d:%02d:%02d:%03d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	fputs(buff,f_debug);
	fputs("\n",f_debug);
	fflush(f_debug);
    va_end(arglist); 
	
	LeaveCriticalSection(&cs_debug);
	#endif
}

void Log(const char *string, ...)
{
	#if defined LOGGING_ON
	if(!f_log) return;
	EnterCriticalSection(&cs_log);
	if(!f_log) return;

	char buff[512];
	va_list arglist; 
    va_start(arglist, string); 
	vsprintf_s(buff,512,string,arglist);

#ifdef PRINT_TO_CONSOLE
	conwindow.Print(string);
#endif

	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(f_log, "[%02d:%02d:%02d:%03d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	fputs(buff,f_log);
	fputs("\n",f_log);
	fflush(f_log);
    va_end(arglist); 

	LeaveCriticalSection(&cs_log);
	#endif
}