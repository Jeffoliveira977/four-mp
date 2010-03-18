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
#include "HandleManager.h"
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

void sq_CreateConVar(HSQUIRRELVM v)
{
	int args = sq_gettop(v);
	if (args < 2)
	{
		sq_pushinteger(v, INVALID_HANDLE);
		return;
	}
	const char *cvarname;
	const char *cvardesc;
	int cvarflags = 0;
	sq_getstring(v, 2, &cvarname);
	if (args >= 3)
	{
		sq_getstring(v, 4, &cvardesc);
	}
	if (args >= 4)
	{
		sq_getinteger(v, 5, &cvarflags);
	}
	switch (sq_gettype(v, 2))
	{
	case OT_STRING:
		{
			const char *cvarvalue;
			sq_getstring(v, 3, &cvarvalue);
			sq_pushinteger(v, vmm.CreateConVar(&v, concore.AddConVar(cvarname, cvarvalue, cvardesc, cvarflags)));
			return;
			break;
		}
	case OT_INTEGER:
		{
			int cvarvalue;
			bool hasMin = false;
			int cvarmin = 0;
			bool hasMax = false;
			int cvarmax = 0;
			sq_getinteger(v, 3, &cvarvalue);
			if (args >= 6)
			{
				sq_getbool(v, 6, (SQBool *)&hasMin);
				sq_getinteger(v, 7, &cvarmin);
			}
			if (args >= 8)
			{
				sq_getbool(v, 6, (SQBool *)&hasMax);
				sq_getinteger(v, 7, &cvarmax);
			}
			sq_pushinteger(v, vmm.CreateConVar(&v, concore.AddConVar(cvarname, cvarvalue, cvardesc, cvarflags, hasMin, cvarmin, hasMax, cvarmax)));
			return;
			break;
		}
	case OT_FLOAT:
		{
			float cvarvalue;
			bool hasMin = false;
			float cvarmin = 0;
			bool hasMax = false;
			float cvarmax = 0;
			sq_getfloat(v, 3, &cvarvalue);
			if (args >= 6)
			{
				sq_getbool(v, 6, (SQBool *)&hasMin);
				sq_getfloat(v, 7, &cvarmin);
			}
			if (args >= 8)
			{
				sq_getbool(v, 6, (SQBool *)&hasMax);
				sq_getfloat(v, 7, &cvarmax);
			}
			sq_pushinteger(v, vmm.CreateConVar(&v, concore.AddConVar(cvarname, cvarvalue, cvardesc, cvarflags, hasMin, cvarmin, hasMax, cvarmax)));
			return;
			break;
		}
	}
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
	vmm.RegServerCmd(&v, cmdcallback, concore.AddConCmd(cmdname, ConCmdDynamic, cmddesc, cmdflags));
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