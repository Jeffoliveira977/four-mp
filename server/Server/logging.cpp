#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logging.h"
#include "../../Shared/Console/ConsoleScreen.h"

extern ConsoleScreen conscreen;

#define DEBUG
#define LOG

void PrintToServer(const wchar_t *string, ...)
{
	va_list arglist; 
    va_start(arglist, string);
	wchar_t *tempstring = (wchar_t *)calloc(_vscwprintf(string, arglist) + 1, sizeof(wchar_t));
	vswprintf(tempstring, string, arglist); 
	conscreen.Print(tempstring);
	free(tempstring);
	#ifdef LOG
		tempstring = new wchar_t[128];
		FILE *f = fopen("server.log", "a");
		time_t now;
		time(&now);
		wcsftime(tempstring, 128, L"[%d.%m %H:%M:%S]", localtime(&now));
		fputws(tempstring, f);
		vfwprintf(f, string, arglist);
		fprintf(f, "\n");
		fclose(f);
		free(tempstring);
	#endif
    va_end(arglist); 
}

void debug(const wchar_t *string, ...)
{
	va_list arglist; 
    va_start(arglist, string); 
	#ifdef DEBUG
		wchar_t *tempstring = (wchar_t *)calloc(_vscwprintf(string, arglist) + 1, sizeof(wchar_t));
		vswprintf(tempstring, string, arglist); 
		conscreen.Print(tempstring);
		free(tempstring);
		tempstring = new wchar_t[128];
		FILE *f = fopen("server.log", "a");
		time_t now;
		time(&now);
		wcsftime(tempstring, 128, L"<%d.%m %H:%M:%S>", localtime(&now));
		fputws(tempstring, f);
		vfwprintf(f, string, arglist);
		fprintf(f, "\n");
		fclose(f);
		free(tempstring);
	#endif
    va_end(arglist); 
}