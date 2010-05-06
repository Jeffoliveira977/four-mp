#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#include "logging.h"
#include "../../Shared/Console/ConsoleScreen.h"

extern ConsoleScreen conscreen;

#define DEBUG
#define LOG

void PrintToServer(const char *string, ...)
{
	va_list arglist; 
    va_start(arglist, string);
	char *tempstring = (char *)calloc(_vscprintf(string, arglist) + 1, sizeof(char));
	vsprintf(tempstring, string, arglist); 
	conscreen.Print(tempstring);
	free(tempstring);
	#ifdef LOG
		FILE *f = fopen("server.log", "a");
		SYSTEMTIME time;
		GetSystemTime(&time);
		fprintf(f, "[%02d.%02d %02d:%02d:%02d] ", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond);
		vfprintf(f, string, arglist);
		fprintf(f, "\n");
		fclose(f);
	#endif
    va_end(arglist); 
}

void debug(const char *string, ...)
{
	va_list arglist; 
    va_start(arglist, string); 
	#ifdef DEBUG
		char *tempstring = (char *)calloc(_vscprintf(string, arglist) + 1, sizeof(char));
		vsprintf(tempstring, string, arglist); 
		conscreen.Print(tempstring);
		free(tempstring);
		FILE *f = fopen("server.log", "a");
		SYSTEMTIME time;
		GetSystemTime(&time);
		fprintf(f, "<%02d.%02d %02d:%02d:%02d> ", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond);
		vfprintf(f, string, arglist);
		fprintf(f, "\n");
		fclose(f);
	#endif
    va_end(arglist); 
}