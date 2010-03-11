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
#include "CoreHandleTypesManager.h"
#include "console\ConsoleCore.h"
#include "console\ConsoleScreen.h"
#include "console\fmpconcommands.h"
#include "VirtualMachineManager.h"

extern CoreHandleTypesManager chtm;
extern ConsoleCore concore;
extern ConsoleScreen conscreen;
extern VirtualMachineManager vmm;

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

void sq_SetScriptName(HSQUIRRELVM v)
{
	const char *string;
	sq_getstring(v, 2, &string);
	vmm.SetVirtualMachineName(&v, string);
}

void sq_SetScriptVersion(HSQUIRRELVM v)
{
	const char *string;
	sq_getstring(v, 2, &string);
	vmm.SetVirtualMachineVersion(&v, string);
}

void sq_SetScriptAuthor(HSQUIRRELVM v)
{
	const char *string;
	sq_getstring(v, 2, &string);
	vmm.SetVirtualMachineAuthor(&v, string);
}

void sq_GetCmdArgs(HSQUIRRELVM v)
{
	sq_pushinteger(v, concore.GetCmdArgs());
}

void sq_GetCmdArgsAsString(HSQUIRRELVM v)
{
	char *arg;
	if (!concore.GetCmdArgString(arg))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushstring(v, arg, -1);
}

void sq_GetCmdArgType(HSQUIRRELVM v)
{
	int argnum;
	ConVarType type;
	sq_getinteger(v, 2, &argnum);
	if (!concore.GetCmdArgType(argnum, type))
	{
		sq_pushnull(v);
		return;
	}
	switch (type)
	{
	case ConVarTypeFloat:
		{
			sq_pushinteger(v, 0);
			break;
		}
	case ConVarTypeInt:
		{
			sq_pushinteger(v, 1);
			break;
		}
	case ConVarTypeString:
		{
			sq_pushinteger(v, 2);
			break;
		}
	}
}

void sq_GetCmdArgString(HSQUIRRELVM v)
{
	int argnum;
	char *arg;
	sq_getinteger(v, 2, &argnum);
	if (!concore.GetCmdArg(argnum, arg))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushstring(v, arg, -1);
}

void sq_GetCmdArgInt(HSQUIRRELVM v)
{
	int argnum;
	int arg;
	sq_getinteger(v, 2, &argnum);
	if (!concore.GetCmdArg(argnum, arg))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushinteger(v, arg);
}

void sq_GetCmdArgFloat(HSQUIRRELVM v)
{
	int argnum;
	float arg;
	sq_getinteger(v, 2, &argnum);
	if (!concore.GetCmdArg(argnum, arg))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushfloat(v, arg);
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
	//TODO
	//chtm.AddScriptCommand(owner, cmdcallback, new ConCmd(cmdname, ConCmdSquirrel, cmddesc, cmdflags));
}

void sq_ServerCommand(HSQUIRRELVM v)
{
	const char *cmdstring;
	sq_getstring(v, 2, &cmdstring);
	concore.InterpretLine(cmdstring);
}

// функция печати - print(string)
void printfunc(HSQUIRRELVM v, const SQChar *s, ...) 
{ 
	va_list arglist; 
    va_start(arglist, s);
	char *tempstring = (char *)calloc(scvprintf(s, arglist) + 1, sizeof(char));
	vsprintf(tempstring, s, arglist); 
	conscreen.Print(tempstring);
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
	conscreen.Print(txt);
	FILE *f = fopen("server.log", "a");
	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(f, "[%02d.%02d %02d:%02d:%02d] ", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond);
	fprintf(f, str);
	fclose(f);
	delete txt, str;
} 