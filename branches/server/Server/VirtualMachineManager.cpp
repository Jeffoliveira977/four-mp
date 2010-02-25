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

#include "console\common.h"
#include "sq.h"

VirtualMachineManager::VirtualMachineManager(void)
{
}

VirtualMachineManager::~VirtualMachineManager(void)
{
	this->UnloadGameMode();
	this->UnloadFilterScripts();
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
	if (vmbuffer[0].loaded)
	{
		return false;
	}
	char *gamemode = (char *)calloc(strlen(string) + 11, sizeof(char));
	sprintf(gamemode, "gamemodes/%s", string);
	if (!this->LoadFilterScriptInternal(0, gamemode))
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
	if (!this->UnloadFilterScriptInternal(0))
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
	unsigned char slots = this->GetNumberOfFreeFilterscriptSlots();
	unsigned char i = 0;
	while ((continuesearch == 0) && (i < slots))
	{
		if (this->LoadFilterScript(data.name))
		{
			i++;
		}
		continuesearch = _findnext(ptr, &data);
	}
	_findclose(ptr);
}

void VirtualMachineManager::UnloadFilterScripts(void)
{
	for (unsigned char i = 1; i <= MAX_FILTERSCRIPTS; i++)
	{
		if (vmbuffer[i].loaded)
		{
			this->UnloadFilterScript(i);
		}
	}
}

bool VirtualMachineManager::LoadFilterScript(const char *string)
{
	unsigned char index;
	if (!this->GetFilterScriptFreeSlot(index))
	{
		return false;
	}
	char *filterscript = (char *)calloc(strlen(string) + 15, sizeof(char));
	sprintf(filterscript, "filterscripts/%s", string);
	if (!this->LoadFilterScriptInternal(index, filterscript))
	{
		return false;
	}
	this->OnFilterScriptInit(index);
	return true;
}

bool VirtualMachineManager::UnloadFilterScript(const unsigned char index)
{
	if (index > MAX_FILTERSCRIPTS)
	{
		return false;
	}
	if (index == 0)
	{
		return false;
	}
	if (!vmbuffer[index].loaded)
	{
		return false;
	}
	this->OnFilterScriptExit(index);
	if (!this->UnloadFilterScriptInternal(index))
	{
		return false;
	}
	return true;
}

bool VirtualMachineManager::GetFilterScriptInfoString(const unsigned char index, char *&string)
{
	if (index > MAX_FILTERSCRIPTS)
	{
		return false;
	}
	if (!vmbuffer[index].loaded)
	{
		return false;
	}
	string = (char *)calloc(_scprintf("%d \"%s\" (%s) by %s", index, vmbuffer[index].name, vmbuffer[index].version, vmbuffer[index].author) + 1, sizeof(char));
	sprintf(string, "%d \"%s\" (%s) by %s", index, vmbuffer[index].name, vmbuffer[index].version, vmbuffer[index].author);
	return true;
}

void VirtualMachineManager::SetFilterScriptName(HSQUIRRELVM *v, const char *string)
{
	unsigned char index;
	if (!this->FindFilterScript(v, index))
	{
		return;
	}
	ResizeStringBuffer(vmbuffer[index].name, strlen(string) + 1);
	strcpy(vmbuffer[index].name, string);
}

void VirtualMachineManager::SetFilterScriptVersion(HSQUIRRELVM *v, const char *string)
{
	unsigned char index;
	if (!this->FindFilterScript(v, index))
	{
		return;
	}
	ResizeStringBuffer(vmbuffer[index].version, strlen(string) + 1);
	strcpy(vmbuffer[index].version, string);
}

void VirtualMachineManager::SetFilterScriptAuthor(HSQUIRRELVM *v, const char *string)
{
	unsigned char index;
	if (!this->FindFilterScript(v, index))
	{
		return;
	}
	ResizeStringBuffer(vmbuffer[index].author, strlen(string) + 1);
	strcpy(vmbuffer[index].author, string);
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

bool VirtualMachineManager::LoadFilterScriptInternal(const unsigned char index, const char *string)
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
			// Script identity functions
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_SetScriptName, "SetScriptName");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_SetScriptVersion, "SetScriptVersion");
			register_global_func(*vmbuffer[index].ptr.squirrel, (SQFUNCTION)sq_SetScriptAuthor, "SetScriptAuthor");
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
	//case VMLanguagePawn:
	//	{
			//Init Pawn
			//int err = aux_LoadProgram(&amx, "test.amx", NULL);
			//if (err != AMX_ERR_NONE)
			//{
			//	//ErrorExit(&amx, err);
			//}
			//pawn_Init(amx);
			//if (err)
			//{
			//	ErrorExit(&amx, err);
			//}
			//err = amx_Exec(&amx, &ret, AMX_EXEC_MAIN);
	//		break;
	//	}
	}
	vmbuffer[index].name = (char *)calloc(16, sizeof(char));
	strcpy(vmbuffer[index].name, "Untitled script");
	vmbuffer[index].version = (char *)calloc(8, sizeof(char));
	strcpy(vmbuffer[index].version, "0.0.0.0");
	vmbuffer[index].author = (char *)calloc(15, sizeof(char));
	strcpy(vmbuffer[index].author, "Unnamed author");
	vmbuffer[index].loaded = true;
	return true;
}

bool VirtualMachineManager::UnloadFilterScriptInternal(const unsigned char index)
{
	if (index > MAX_FILTERSCRIPTS)
	{
		return false;
	}
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
	free(vmbuffer[index].name);
	free(vmbuffer[index].version);
	free(vmbuffer[index].author);
	vmbuffer[index].loaded = false;
	return true;
}

unsigned char VirtualMachineManager::GetNumberOfFreeFilterscriptSlots(void)
{
	unsigned char slots = 0;
	for (unsigned char i = 0; i <= MAX_FILTERSCRIPTS; i++)
	{
		if (!vmbuffer[i].loaded)
		{
			slots++;
		}
	}
	return slots;
}

bool VirtualMachineManager::GetFilterScriptFreeSlot(unsigned char &index)
{
	for (index = 1; index <= MAX_FILTERSCRIPTS; index++)
	{
		if (!vmbuffer[index].loaded)
		{
			return true;
		}
	}
	return false;
}

bool VirtualMachineManager::FindFilterScript(HSQUIRRELVM *v, unsigned char &index)
{
	for (index = 0; index <= MAX_FILTERSCRIPTS; index++)
	{
		if ((vmbuffer[index].loaded) && (vmbuffer[index].lang == VMLanguageSquirrel) && (*vmbuffer[index].ptr.squirrel == *v))
		{
			return true;
		}
	}
	return false;
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
	if (index > MAX_FILTERSCRIPTS)
	{
		return;
	}
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

void VirtualMachineManager::OnFilterScriptExit(const unsigned char index)
{
	if (index > MAX_FILTERSCRIPTS)
	{
		return;
	}
	if (!vmbuffer[index].loaded)
	{
		return;
	}
	switch (vmbuffer[index].lang)
	{
	case VMLanguageSquirrel:
		{
			sc_OnFilterScriptExit(*vmbuffer[index].ptr.squirrel);
			break;
		}
	}
}