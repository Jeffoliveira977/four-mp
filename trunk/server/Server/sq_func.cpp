/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "sq/squirrel.h"
#include "VirtualMachineManager.h"

extern VirtualMachineManager vmm;

// ������� ����������� �������
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
	unsigned char index;
	if (!vmm.FindVirtualMachine(&v, index))
	{
		return;
	}
	vmm.SetVirtualMachineName(index, string);
}

void sq_SetScriptVersion(HSQUIRRELVM v)
{
	const char *string;
	sq_getstring(v, 2, &string);
	unsigned char index;
	if (!vmm.FindVirtualMachine(&v, index))
	{
		return;
	}
	vmm.SetVirtualMachineVersion(index, string);
}

void sq_SetScriptAuthor(HSQUIRRELVM v)
{
	const char *string;
	sq_getstring(v, 2, &string);
	unsigned char index;
	if (!vmm.FindVirtualMachine(&v, index))
	{
		return;
	}
	vmm.SetVirtualMachineAuthor(index, string);
}