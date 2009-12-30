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

extern HSQUIRRELVM v;

#ifdef SQUNICODE 
#define scvprintf vwprintf 
#else 
#define scvprintf vprintf 
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

// функция печати - print(string)
void printfunc(HSQUIRRELVM v, const SQChar *s, ...) 
{ 
	va_list arglist; 
    va_start(arglist, s); 
    scvprintf(s, arglist); 
    va_end(arglist); 
} 

// Функция для печати русских строк - printr(string)
void sq_printr(HSQUIRRELVM v) 
{ 
	char *txt = new char[1024];
	const char *str = new char[1024];
	sq_getstring(v, 2, &str);
	CharToOem(str, txt);
	printf(txt);
	delete txt, str;
} 