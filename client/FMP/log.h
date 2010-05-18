/*
*	Created:			26.10.09
*	Created by:			009 & WNeZRoS
*	Last Modifed:		28.10.09
*/
#pragma once

#define DEBUG_ON
#define LOGGING_ON
//#define PRINT_TO_CONSOLE

void InitLogs();
void CloseLogs();

bool FileExists(const char *fname);

void PrintToConsole(const char *string, ...);

void Debug(const char *string, ...);
void Log(const char *string, ...);
