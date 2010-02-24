#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VirtualMachineManager.h"
#include "sq\sqstdaux.h"
#include "sq\sqstdblob.h"
#include "sq\sqstdio.h"
#include "sq\sqstdmath.h"
#include "sq\sqstdstring.h"
#include "sq\sqstdsystem.h"

#include "sq.h"

VirtualMachineManager::VirtualMachineManager(void)
{
}

VirtualMachineManager::~VirtualMachineManager(void)
{
}

bool VirtualMachineManager::LoadGameMode(const char *string)
{
	if (vmbuffer[0].loaded)
	{
		return false;
	}
	VirtualMachineLanguage lang;
	if (strcmp(strrchr(string, '.') + 1, "nut") == 0)
	{
		lang = VMLanguageSquirrel;
	}
	else
	{
		return false;
	}
	if(!this->InitVirtualMachine(0, lang))
	{
		return false;
	}
	char *gamemode = (char *)calloc(strlen(string) + 11, sizeof(char));
	sprintf(gamemode, "gamemodes/%s", string);
	if(!SQ_SUCCEEDED(sqstd_dofile(*vmbuffer[0].ptr.squirrel, _SC(gamemode), 0, 1))) 
    {
		return false;
    }
	vmbuffer[0].loaded = true;
	return true;
}

void VirtualMachineManager::OnGameModeInit(void)
{
	for (unsigned char i = 0; i <= MAX_FILTERSCRIPTS; i++)
	{
		if (vmbuffer[i].loaded)
		{
			switch (vmbuffer[i].lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnGameModeInit(*vmbuffer[i].ptr.squirrel);
					break;
				}
			}
		}
	}
}

void VirtualMachineManager::OnGameModeExit(void)
{
	for (unsigned char i = 0; i <= MAX_FILTERSCRIPTS; i++)
	{
		if (vmbuffer[i].loaded)
		{
			switch (vmbuffer[i].lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnGameModeExit(*vmbuffer[i].ptr.squirrel);
					break;
				}
			}
		}
	}
}

int VirtualMachineManager::OnPlayerConnect(int playerid, char name[32])
{
	for (unsigned char i = 0; i <= MAX_FILTERSCRIPTS; i++)
	{
		if (vmbuffer[i].loaded)
		{
			switch (vmbuffer[i].lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnPlayerConnect(*vmbuffer[i].ptr.squirrel, playerid, name);
					break;
				}
			}
		}
	}
	return 1; //Temp
}

void VirtualMachineManager::OnPlayerDisconnect(int playerid)
{
	for (unsigned char i = 0; i <= MAX_FILTERSCRIPTS; i++)
	{
		if (vmbuffer[i].loaded)
		{
			switch (vmbuffer[i].lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnPlayerDisconnect(*vmbuffer[i].ptr.squirrel, playerid);
					break;
				}
			}
		}
	}
}

void VirtualMachineManager::OnPlayerSpawn(int playerid, int cl)
{
	for (unsigned char i = 0; i <= MAX_FILTERSCRIPTS; i++)
	{
		if (vmbuffer[i].loaded)
		{
			switch (vmbuffer[i].lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnPlayerSpawn(*vmbuffer[i].ptr.squirrel, playerid, cl);
					break;
				}
			}
		}
	}
}

bool VirtualMachineManager::InitVirtualMachine(const unsigned char index, const VirtualMachineLanguage lang)
{
	if (index > MAX_FILTERSCRIPTS)
	{
		return false;
	}
	if (vmbuffer[index].loaded)
	{
		return false;
	}
	vmbuffer[index].lang = lang;
	switch (lang)
	{
	case VMLanguageSquirrel:
		{
			vmbuffer[index].ptr.squirrel = new HSQUIRRELVM;
			// Init Squirrel
			*vmbuffer[index].ptr.squirrel = sq_open(1024);
			sqstd_seterrorhandlers(*vmbuffer[index].ptr.squirrel);
			sq_setprintfunc(*vmbuffer[index].ptr.squirrel, printfunc);
			// Register Script Funcions
			// Console functions
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_GetCmdArgs, "GetCmdArgs");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_GetCmdArgsAsString, "GetCmdArgsAsString");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_GetCmdArgType, "GetCmdArgType");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_GetCmdArgString, "GetCmdArgString");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_GetCmdArgInt, "GetCmdArgInt");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_GetCmdArgFloat, "GetCmdArgFloat");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_printr, "printr");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_RegServerCmd, "RegServerCmd");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_ServerCommand, "ServerCommand");
			// Car functions
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_CreateCar, "CreateCar");
			// Player functions
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_GiveWeapon, "GiveWeapon");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_addPlayerClass, "addPlayerClass");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_enableComponentSelect, "enableComponentSelect");
			sq_pushroottable(*vmbuffer[index].ptr.squirrel);
			// Register Standart Script Functions
			sqstd_register_stringlib(*vmbuffer[index].ptr.squirrel);
			sqstd_register_mathlib(*vmbuffer[index].ptr.squirrel);
			sqstd_register_systemlib(*vmbuffer[index].ptr.squirrel);
			sqstd_seterrorhandlers(*vmbuffer[index].ptr.squirrel);
			break;
		}
	}
	return true;
}