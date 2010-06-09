#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logging.h"
#include "../../Shared/Console/ConsoleScreen.h"

extern ConsoleScreen conscreen;

void PrintToConsole(const wchar_t *string, ...)
{
	va_list arglist;
    va_start(arglist, string);
	int length = _vscwprintf(string, arglist) + 1;
	wchar_t *tempstring = (wchar_t *)calloc(length, sizeof(wchar_t));
	vswprintf(tempstring, length, string, arglist); 
	conscreen.Print(tempstring);
	free(tempstring);
	va_end(arglist); 
}