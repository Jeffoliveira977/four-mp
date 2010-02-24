#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

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

bool VirtualMachineManager::IsGameModeLoaded(void)
{
	if (!vmbuffer[0].loaded)
	{
		return false;
	}
	return true;
}

bool VirtualMachineManager::LoadGameMode(const char *string)
{
	if (this->IsGameModeLoaded())
	{
		return false;
	}
	char *gamemode = (char *)calloc(strlen(string) + 11, sizeof(char));
	sprintf(gamemode, "gamemodes/%s", string);
	if (!this->LoadFilterScript(0, gamemode))
	{
		return false;
	}
	this->OnGameModeInit();
	return true;
}

bool VirtualMachineManager::UnloadGameMode(void)
{
	if (!this->IsGameModeLoaded())
	{
		return false;
	}
	this->OnGameModeExit();
	if (!this->UnloadFilterScript(0))
	{
		return false;
	}
	return true;
}

void VirtualMachineManager::LoadFilterScripts(void)
{
	intptr_t ptr;
	_finddata64i32_t data;
	ptr = _findfirst("filterscripts\\*.*", &data);
	if (ptr == -1)
	{
		return;
	}
	_findnext(ptr, &data);
	int continuesearch = _findnext(ptr, &data);
	char * filename;
	unsigned char i = 1;
	while ((continuesearch == 0) && (i <= MAX_FILTERSCRIPTS))
	{
		filename = (char *)calloc(strlen(data.name) + 15, sizeof(char));
		sprintf(filename, "filterscripts/%s", data.name);
		if (this->LoadFilterScript(i, filename))
		{
			this->OnFilterScriptInit(i);
			i++;
		}
		continuesearch = _findnext(ptr, &data);
	}
	_findclose(ptr);
}

void VirtualMachineManager::UnloadFilterScripts(void)
{

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

bool VirtualMachineManager::LoadFilterScript(const unsigned char index, const char *string)
{
	if (index > MAX_FILTERSCRIPTS)
	{
		return false;
	}
	if (vmbuffer[index].loaded)
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
			if(!SQ_SUCCEEDED(sqstd_dofile(*vmbuffer[index].ptr.squirrel, _SC(string), 0, 1))) 
			{
				sq_close(*vmbuffer[index].ptr.squirrel);
				delete vmbuffer[index].ptr.squirrel;
				return false;
			}
			vmbuffer[index].lang = VMLanguageSquirrel;
			break;
		}
	}
	vmbuffer[index].loaded = true;
	return true;
}

bool VirtualMachineManager::UnloadFilterScript(const unsigned char index)
{
	if (!vmbuffer[index].loaded)
	{
		return false;
	}
	switch (vmbuffer[index].lang)
	{
	case VMLanguageSquirrel:
		{
			sq_close(*vmbuffer[index].ptr.squirrel);
			delete vmbuffer[index].ptr.squirrel;
			break;
		}
	}
	vmbuffer[index].loaded = false;
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

void VirtualMachineManager::OnFilterScriptInit(const unsigned char index)
{
	if (!vmbuffer[index].loaded)
	{
		return;
	}
	switch (vmbuffer[index].lang)
	{
	case VMLanguageSquirrel:
		{
			sc_OnFilterScriptInit(*vmbuffer[index].ptr.squirrel);
			break;
		}
	}
}