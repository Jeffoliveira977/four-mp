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

bool FileExists(const char *fname)
{
	return _access(fname, 0) != -1;
}

void PrintToConsole(const char *string, ...)
{
	va_list arglist; 
    va_start(arglist, string);
	char *tempstring = (char *)calloc(_vscprintf(string, arglist) + 1, sizeof(char));
	vsprintf(tempstring, string, arglist); 
	conwindow.Print(tempstring);
	free(tempstring);
	va_end(arglist); 
}

void debug(const char* string)
{
	#if defined DEBUG_ON
	conwindow.Print(string);
	FILE* temp;
	temp = fopen("FMP_debug.txt","a");
	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(temp, "[%02d:%02d:%02d:%02d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	fputs(string,temp);
	fputs("\n",temp);
	fclose(temp);
	#endif
}

void debug_clear()
{
	#if defined DEBUG_ON
	if(FileExists("FMP_debug.txt")) DeleteFile("FMP_debug.txt");
	#endif
}

void log(const char* string)
{
	#if defined LOGGING_ON
	conwindow.Print(string);
	FILE* temp;
	temp = fopen("FMP_log.txt","a");
	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(temp, "[%02d:%02d:%02d:%02d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	fputs(string,temp);
	fputs("\n",temp);
	fclose(temp);
	#endif
}

void log_clear()
{
	#if defined LOGGING_ON
	if(FileExists("FMP_log.txt")) DeleteFile("FMP_log.txt");
	#endif
}

void Debug(const char *string, ...)
{
	#if defined DEBUG_ON
	char buff[512];
	va_list arglist; 
    va_start(arglist, string); 
	vsprintf_s(buff,512,string,arglist);
	conwindow.Print(buff);
	FILE* temp;
	temp = fopen("FMP_debug.txt","a");
	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(temp, "[%02d:%02d:%02d:%03d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	fputs(buff,temp);
	fputs("\n",temp);
	fclose(temp);
    va_end(arglist); 
	#endif
}

void DebugEx(const char *file, const char *string, ...)
{
	#if defined DEBUG_ON
	char buff[512];
	va_list arglist; 
    va_start(arglist, string); 
	vsprintf_s(buff,512,string,arglist);
	conwindow.Print(buff);
	FILE* temp;
	temp = fopen(file,"a");
	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(temp, "[%02d:%02d:%02d:%03d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	fputs(buff,temp);
	fputs("\n",temp);
	fclose(temp);
    va_end(arglist); 
	#endif
}

void Log(const char *string, ...)
{
	#if defined LOGGING_ON
	char buff[512];
	va_list arglist; 
    va_start(arglist, string); 
	vsprintf_s(buff,512,string,arglist);
	conwindow.Print(buff);
	FILE* temp;
	temp = fopen("FMP_log.txt","a");
	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(temp, "[%02d:%02d:%02d:%03d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	fputs(buff,temp);
	fputs("\n",temp);
	fclose(temp);
    va_end(arglist); 
	#endif
}