#include <stdio.h>
#include <stdarg.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "main.h"
#include "sq_consolenatives.h"
#include "HandleManager.h"
#include "console\ConsoleCore.h"
#include "console\fmpconcommands.h"
#include "VirtualMachineManager.h"

extern ConsoleCore concore;
extern VirtualMachineManager vmm;

#ifdef SQUNICODE
#define scvprintf _vscwprintf
#else
#define scvprintf _vscprintf
#endif

// функция печати - PrintToServer(string)
void sq_PrintToServer(HSQUIRRELVM v, const SQChar *s, ...)
{
	va_list arglist;
    va_start(arglist, s);
	char *tempstring = (char *)calloc(scvprintf(s, arglist) + 1, sizeof(char));
	vsprintf(tempstring, s, arglist);
	PrintToServer(tempstring);
	free(tempstring);
    va_end(arglist);
}

// Функция для печати русских строк - printr(string)
void sq_printr(HSQUIRRELVM v)
{
	char *txt = new char[1024];
	const char *str = new char[1024];
	sq_getstring(v, 2, &str);
	CharToOem(str, txt);
	PrintToServer(txt);
	delete txt, str;
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

void sq_FindConVar(HSQUIRRELVM v)
{
	const char *cvarname;
	sq_getstring(v, 2, &cvarname);
	sq_pushinteger(v, vmm.FindConVar(&v, cvarname));
}

void sq_ResetConVar(HSQUIRRELVM v)
{
	int cvarhandle;
	sq_getinteger(v, 2, &cvarhandle);
	sq_pushbool(v, vmm.ResetConVar(&v, cvarhandle));
}

void sq_GetConVarName(HSQUIRRELVM v)
{
	int cvarhandle;
	sq_getinteger(v, 2, &cvarhandle);
	char *cvarname = vmm.GetConVarName(&v, cvarhandle);
	if (cvarname == NULL)
	{
		sq_pushnull(v);
		return;
	}
	sq_pushstring(v, cvarname, -1);
	free(cvarname);
}

void sq_GetConVarFloat(HSQUIRRELVM v)
{
	int cvarhandle;
	float cvarvalue;
	sq_getinteger(v, 2, &cvarhandle);
	if (!vmm.GetConVarValue(&v, cvarhandle, cvarvalue))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushfloat(v, cvarvalue);
}

void sq_GetConVarInt(HSQUIRRELVM v)
{
	int cvarhandle;
	int cvarvalue;
	sq_getinteger(v, 2, &cvarhandle);
	if (!vmm.GetConVarValue(&v, cvarhandle, cvarvalue))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushinteger(v, cvarvalue);
}

void sq_GetConVarString(HSQUIRRELVM v)
{
	int cvarhandle;
	char *cvarvalue;
	sq_getinteger(v, 2, &cvarhandle);
	if (!vmm.GetConVarValue(&v, cvarhandle, cvarvalue))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushstring(v, cvarvalue, -1);
	free(cvarvalue);
}

void sq_GetConVarFlags(HSQUIRRELVM v)
{
	int cvarhandle;
	int cvarflags;
	sq_getinteger(v, 2, &cvarhandle);
	if (!vmm.GetConVarFlags(&v, cvarhandle, cvarflags))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushinteger(v, cvarflags);
}

void sq_GetConVarBoundFloat(HSQUIRRELVM v)
{
	int cvarhandle;
	int type;
	float cvarbound;
	sq_getinteger(v, 2, &cvarhandle);
	sq_getinteger(v, 3, &type);
	if (!vmm.GetConVarBound(&v, cvarhandle, (ConVarBoundType)type, cvarbound))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushfloat(v, cvarbound);
}

void sq_GetConVarBoundInt(HSQUIRRELVM v)
{
	int cvarhandle;
	int type;
	int cvarbound;
	sq_getinteger(v, 2, &cvarhandle);
	sq_getinteger(v, 3, &type);
	if (!vmm.GetConVarBound(&v, cvarhandle, (ConVarBoundType)type, cvarbound))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushinteger(v, cvarbound);
}

void sq_SetConVarFloat(HSQUIRRELVM v)
{
	int cvarhandle;
	float cvarvalue;
	sq_getinteger(v, 2, &cvarhandle);
	sq_getfloat(v, 3, &cvarvalue);
	if (!vmm.SetConVarValue(&v, cvarhandle, cvarvalue))
	{
		sq_pushbool(v, false);
		return;
	}
	sq_pushbool(v, true);
}

void sq_SetConVarInt(HSQUIRRELVM v)
{
	int cvarhandle;
	int cvarvalue;
	sq_getinteger(v, 2, &cvarhandle);
	sq_getinteger(v, 3, &cvarvalue);
	if (!vmm.SetConVarValue(&v, cvarhandle, cvarvalue))
	{
		sq_pushbool(v, false);
		return;
	}
	sq_pushbool(v, true);
}

void sq_SetConVarString(HSQUIRRELVM v)
{
	int cvarhandle;
	const char *cvarvalue;
	sq_getinteger(v, 2, &cvarhandle);
	sq_getstring(v, 3, &cvarvalue);
	if (!vmm.SetConVarValue(&v, cvarhandle, cvarvalue))
	{
		sq_pushbool(v, false);
		return;
	}
	sq_pushbool(v, true);
}

void sq_SetConVarFlags(HSQUIRRELVM v)
{
	int cvarhandle;
	int cvarflags;
	sq_getinteger(v, 2, &cvarhandle);
	sq_getinteger(v, 3, &cvarflags);
	if (!vmm.SetConVarFlags(&v, cvarhandle, cvarflags))
	{
		sq_pushbool(v, false);
		return;
	}
	sq_pushbool(v, true);
}

void sq_SetConVarBoundFloat(HSQUIRRELVM v)
{
	int cvarhandle;
	int type;
	float cvarbound;
	sq_getinteger(v, 2, &cvarhandle);
	sq_getinteger(v, 3, &type);
	sq_getfloat(v, 4, &cvarbound);
	if (!vmm.SetConVarBound(&v, cvarhandle, (ConVarBoundType)type, cvarbound))
	{
		sq_pushbool(v, false);
		return;
	}
	sq_pushbool(v, true);
}

void sq_SetConVarBoundInt(HSQUIRRELVM v)
{
	int cvarhandle;
	int type;
	int cvarbound;
	sq_getinteger(v, 2, &cvarhandle);
	sq_getinteger(v, 3, &type);
	sq_getinteger(v, 4, &cvarbound);
	if (!vmm.SetConVarBound(&v, cvarhandle, (ConVarBoundType)type, cvarbound))
	{
		sq_pushbool(v, false);
		return;
	}
	sq_pushbool(v, true);
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