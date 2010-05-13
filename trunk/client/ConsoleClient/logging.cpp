#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logging.h"
#include "../../Shared/Console/ConsoleScreen.h"

extern ConsoleScreen conscreen;

void PrintToConsole(const char *string, ...)
{
	va_list arglist; 
    va_start(arglist, string);
	char *tempstring = (char *)calloc(_vscprintf(string, arglist) + 1, sizeof(char));
	vsprintf(tempstring, string, arglist); 
	conscreen.Print(tempstring);
	free(tempstring);
	va_end(arglist); 
}