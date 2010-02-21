/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include <iostream>
#include <windows.h>
#include <stdarg.h>
#include "sq\squirrel.h"
#include "sq\sqstdsystem.h"
#include "main.h"
#include "Console.h"
#include "concommands.h"
#include "ScriptCommandHandler.h"

extern Console con;
extern ScriptCommandHandler cmdhandler;
extern HSQUIRRELVM v;

#ifdef SQUNICODE 
#define scvprintf _vscwprintf 
#else 
#define scvprintf _vscprintf 
#endif 

// функция регистрации функций
SQInteger register_global_func(HSQUIRRELVM v,SQFUNCTION f,const char *fname)
{
    sq_pushroottable(v);
    sq_pushstring(v,fname,-1);
    sq_newclosure(v,f,0); 
    sq_createslot(v,-3); 
    sq_pop(v,1);   
	return 1;
}

void sq_RegServerCmd(HSQUIRRELVM v)
{
	const char *cmdname;
	const char *cmdcallback;
	const char *cmddesc;
	int cmdflags;
	sq_getstring(v, 2, &cmdname);
	sq_getstring(v, 3, &cmdcallback);
	sq_getstring(v, 4, &cmddesc);
	sq_getinteger(v, 5, &cmdflags);
	new ConCmd(cmdname, ConCmdSquirrel, cmddesc, cmdflags);
	cmdhandler.AddScriptCommand(cmdname, cmdcallback);
}

// функция печати - print(string)
void printfunc(HSQUIRRELVM v, const SQChar *s, ...) 
{ 
	va_list arglist; 
    va_start(arglist, s);
	char *tempstring = (char *)calloc(scvprintf(s, arglist) + 1, sizeof(char));
	vsprintf(tempstring, s, arglist); 
	con.Print(tempstring);
	free(tempstring);
	FILE *f = fopen("server.log", "a");
	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(f, "[%02d.%02d %02d:%02d:%02d] ", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond);
	vfprintf(f, s, arglist);
	fclose(f);

    va_end(arglist); 
} 

// Функция для печати русских строк - printr(string)
void sq_printr(HSQUIRRELVM v) 
{ 
	char *txt = new char[1024];
	const char *str = new char[1024];
	sq_getstring(v, 2, &str);
	CharToOem(str, txt);
	con.Print(txt);
	FILE *f = fopen("server.log", "a");
	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(f, "[%02d.%02d %02d:%02d:%02d] ", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond);
	fprintf(f, str);
	fclose(f);
	delete txt, str;
} 