/*
*	Created:			26.10.09
*	Created by:			009 & WNeZRoS
*	Last Modifed:		28.10.09
*/
#pragma once

//#define DEBUG_ON
#define LOGGING_ON

bool FileExists(const char *fname);
void PrintToConsole(const char *string, ...);
void debug(const char* string);
void DebugEx(const char *file, const char *string, ...);
void debug_clear();
void log(const char* string);
void log_clear();
void Debug(const char *string, ...);
void Log(const char *string, ...);
