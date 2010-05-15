#include <stdio.h>
#include <stdarg.h>
#include <time.h>

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
		tempstring = new char[128];
		FILE *f = fopen("server.log", "a");
		time_t now;
		time(&now);
		strftime(tempstring, 128, "[%d.%m %H:%M:%S]", localtime(&now));
		fputs(tempstring, f);
		vfprintf(f, string, arglist);
		fprintf(f, "\n");
		fclose(f);
		free(tempstring);
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

		tempstring = new char[128];
		FILE *f = fopen("server.log", "a");
		time_t now;
		time(&now);
		strftime(tempstring, 128, "<%d.%m %H:%M:%S>", localtime(&now));
		fputs(tempstring, f);
		vfprintf(f, string, arglist);
		fprintf(f, "\n");
		fclose(f);
		free(tempstring);
	#endif
    va_end(arglist); 
}