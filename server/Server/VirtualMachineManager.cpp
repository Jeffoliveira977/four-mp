/// \file
/// \brief Source file that contains implementation of the VirtualMachineManager class.
/// \details See class description.
/// \author FaTony. Wrapped around initial WNeZRoS' code.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include "sq/squirrel.h"
#include "sq/sqstdaux.h"
#include "sq/sqstdblob.h"
#include "sq/sqstdio.h"
#include "sq/sqstdmath.h"
#include "sq/sqstdstring.h"
#include "sq/sqstdsystem.h"

#include "VirtualMachineManager.h"
#include "../../Shared/Console/common.h"
#include "HandleManager.h"
#include "CoreHandleTypesManager.h"

#include "sq.h"
#include "sq_consolenatives.h"
#include "sq_playernatives.h"

extern HandleManager hm;
extern CoreHandleTypesManager chtm;

VirtualMachineManager::VirtualMachineManager(void)
{
	maxvmbuffersize = 17;
	vmbuffer = (VirtualMachine **)calloc(1, sizeof(VirtualMachine *));
	vmbuffer[0] = NULL;
	vmbuffersize = 0;
}

VirtualMachineManager::~VirtualMachineManager(void)
{
	this->UnloadGameMode();
	this->UnloadFilterScripts();
	free(vmbuffer);
}

bool VirtualMachineManager::IsGameModeLoaded(void)
{
	if (vmbuffer[0] == NULL)
	{
		return false;
	}
	return true;
}

bool VirtualMachineManager::LoadGameMode(const char *string)
{
	if (vmbuffer[0] != NULL)
	{
		return false;
	}
	int length = strlen(string);
	char *gamemode = (char *)calloc(length + 11, sizeof(char));
	sprintf(gamemode, "gamemodes/%s", string);
	if (!this->LoadVirtualMachine(0, gamemode))
	{
		return false;
	}
	vmbuffer[0]->filename = (char *)calloc(length + 1, sizeof(char));
	strcpy(vmbuffer[0]->filename, string);
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
	if (!this->UnloadVirtualMachine(0))
	{
		return false;
	}
	free(vmbuffer[0]->filename);
	delete vmbuffer[0];
	vmbuffer[0] = NULL;
	return true;
}

char *VirtualMachineManager::GetGameModeName(void)
{
	if (!this->IsGameModeLoaded())
	{
		return NULL;
	}
	char *name = (char *)calloc(strlen(vmbuffer[0]->name) + 1, sizeof(char));
	strcpy(name, vmbuffer[0]->name);
	return name;
}

unsigned char VirtualMachineManager::GetMaxVirtualMachineBufferSize(void)
{
	return maxvmbuffersize;
}

unsigned char VirtualMachineManager::GetVirtualMachineBufferSize(void)
{
	return vmbuffersize;
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
	unsigned char slots = this->GetNumberOfFreeFilterScriptSlots();
	unsigned char i = 0;
	while ((continuesearch == 0) && (i < slots))
	{
		if ((!this->IsFilterScriptLoaded(data.name)) && (this->LoadFilterScript(data.name)))
		{
			i++;
		}
		continuesearch = _findnext(ptr, &data);
	}
	_findclose(ptr);
}

void VirtualMachineManager::UnloadFilterScripts(void)
{
	for (unsigned char i = 1; i < vmbuffersize; i++)
	{
		if (vmbuffer[i] != NULL)
		{
			this->UnloadFilterScript(i);
		}
	}
}

void VirtualMachineManager::ReloadFilterScripts(void)
{
	for (unsigned char i = 1; i < vmbuffersize; i++)
	{
		if (vmbuffer[i] != NULL)
		{
			this->ReloadFilterScript(i);
		}
	}
}

void VirtualMachineManager::PauseVirtualMachines(void)
{
	for (unsigned char i = 0; i < vmbuffersize; i++)
	{
		if (vmbuffer[i] != NULL)
		{
			this->PauseVirtualMachine(i);
		}
	}
}

void VirtualMachineManager::UnpauseVirtualMachines(void)
{
	for (unsigned char i = 0; i < vmbuffersize; i++)
	{
		if (vmbuffer[i] != NULL)
		{
			this->UnpauseVirtualMachine(i);
		}
	}
}

bool VirtualMachineManager::IsFilterScriptLoaded(const char *string)
{
	for (unsigned char i = 1; i < vmbuffersize; i++)
	{
		if ((vmbuffer[i] != NULL) && (strcmp(vmbuffer[i]->filename, string) == 0))
		{
			return true;
		}
	}
	return false;
}

bool VirtualMachineManager::LoadFilterScript(const char *string)
{
	unsigned char index;
	if (!this->GetFilterScriptFreeSlot(index))
	{
		return false;
	}
	if (!this->LoadFilterScriptInternal(index, string))
	{
		return false;
	}
	return true;
}

bool VirtualMachineManager::UnloadFilterScript(const unsigned char index)
{
	if (index >= vmbuffersize)
	{
		return false;
	}
	if (index == 0)
	{
		return false;
	}
	if (vmbuffer[index] == NULL)
	{
		return false;
	}
	this->OnFilterScriptExit(index);
	if (!this->UnloadVirtualMachine(index))
	{
		return false;
	}
	free(vmbuffer[index]->filename);
	delete vmbuffer[index];
	vmbuffer[index] = NULL;
	return true;
}

bool VirtualMachineManager::ReloadFilterScript(const unsigned char index)
{
	if (index >= vmbuffersize)
	{
		return false;
	}
	if (vmbuffer[index] == NULL)
	{
		return false;
	}
	char *filename = (char *)calloc(strlen(vmbuffer[index]->filename) + 1, sizeof(char));
	strcpy(filename, vmbuffer[index]->filename);
	if (!this->UnloadFilterScript(index))
	{
		return false;
	}
	if (!this->LoadFilterScriptInternal(index, filename))
	{
		return false;
	}
	return true;
}

bool VirtualMachineManager::PauseVirtualMachine(const unsigned char index)
{
	if (index >= vmbuffersize)
	{
		return false;
	}
	if (vmbuffer[index] == NULL)
	{
		return false;
	}
	if (vmbuffer[index]->paused)
	{
		return false;
	}
	vmbuffer[index]->paused = true;
	return true;
}

bool VirtualMachineManager::UnpauseVirtualMachine(const unsigned char index)
{
	if (index >= vmbuffersize)
	{
		return false;
	}
	if (vmbuffer[index] == NULL)
	{
		return false;
	}
	if (!vmbuffer[index]->paused)
	{
		return false;
	}
	vmbuffer[index]->paused = false;
	return true;
}

char *VirtualMachineManager::GetVirtualMachineInfoString(const unsigned char index)
{
	if (index >= vmbuffersize)
	{
		return NULL;
	}
	if (vmbuffer[index] == NULL)
	{
		return NULL;
	}
	char *string;
	if (!vmbuffer[index]->paused)
	{
		string = (char *)calloc(_scprintf("%d \"%s\" (%s) by %s", index, vmbuffer[index]->name, vmbuffer[index]->version, vmbuffer[index]->author) + 1, sizeof(char));
		sprintf(string, "%d \"%s\" (%s) by %s", index, vmbuffer[index]->name, vmbuffer[index]->version, vmbuffer[index]->author);
	}
	else
	{
		string = (char *)calloc(_scprintf("%d (Paused) \"%s\" (%s) by %s", index, vmbuffer[index]->name, vmbuffer[index]->version, vmbuffer[index]->author) + 1, sizeof(char));
		sprintf(string, "%d (Paused) \"%s\" (%s) by %s", index, vmbuffer[index]->name, vmbuffer[index]->version, vmbuffer[index]->author);
	}
	return string;
}

bool VirtualMachineManager::FindVirtualMachine(const HSQUIRRELVM *v, unsigned char &index)
{
	for (index = 0; index < vmbuffersize; index++)
	{
		if ((vmbuffer[index] != NULL) && (vmbuffer[index]->lang == VMLanguageSquirrel) && (*vmbuffer[index]->ptr.squirrel == *v))
		{
			return true;
		}
	}
	return false;
}

void VirtualMachineManager::SetVirtualMachineName(const unsigned char index, const char *string)
{
	if (index >= vmbuffersize)
	{
		return;
	}
	if (vmbuffer[index] == NULL)
	{
		return;
	}
	ResizeBuffer<char *>(vmbuffer[index]->name, strlen(string) + 1);
	strcpy(vmbuffer[index]->name, string);
}

void VirtualMachineManager::SetVirtualMachineVersion(const unsigned char index, const char *string)
{
	if (index >= vmbuffersize)
	{
		return;
	}
	if (vmbuffer[index] == NULL)
	{
		return;
	}
	ResizeBuffer<char *>(vmbuffer[index]->version, strlen(string) + 1);
	strcpy(vmbuffer[index]->version, string);
}

void VirtualMachineManager::SetVirtualMachineAuthor(const unsigned char index, const char *string)
{
	if (index >= vmbuffersize)
	{
		return;
	}
	if (vmbuffer[index] == NULL)
	{
		return;
	}
	ResizeBuffer<char *>(vmbuffer[index]->author, strlen(string) + 1);
	strcpy(vmbuffer[index]->author, string);
}

bool VirtualMachineManager::OnPlayerConnect(const short index, const char *name)
{
	for (unsigned char i = 0; i < vmbuffersize; i++)
	{
		if ((vmbuffer[i] != NULL) && (!vmbuffer[i]->paused))
		{
			switch (vmbuffer[i]->lang)
			{
			case VMLanguageSquirrel:
				{
					if (!sc_OnPlayerConnect(*vmbuffer[i]->ptr.squirrel, index, name))
					{
						return false;
					}
					break;
				}
			}
		}
	}
	return true;
}

void VirtualMachineManager::OnPlayerDisconnect(const short index)
{
	for (unsigned char i = 0; i < vmbuffersize; i++)
	{
		if ((vmbuffer[i] != NULL) && (!vmbuffer[i]->paused))
		{
			switch (vmbuffer[i]->lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnPlayerDisconnect(*vmbuffer[i]->ptr.squirrel, index);
					break;
				}
			}
		}
	}
}

void VirtualMachineManager::OnPlayerSpawn(const short playerindex)
{
	for (unsigned char i = 0; i < vmbuffersize; i++)
	{
		if ((vmbuffer[i] != NULL) && (!vmbuffer[i]->paused))
		{
			switch (vmbuffer[i]->lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnPlayerSpawn(*vmbuffer[i]->ptr.squirrel, playerindex);
					break;
				}
			}
		}
	}
}

bool VirtualMachineManager::OnPlayerText(const short playerindex, const char *data)
{
	for (unsigned char i = 0; i < vmbuffersize; i++)
	{
		if ((vmbuffer[i] != NULL) && (!vmbuffer[i]->paused))
		{
			switch (vmbuffer[i]->lang)
			{
			case VMLanguageSquirrel:
				{
					if (!sc_OnPlayerText(*vmbuffer[i]->ptr.squirrel, playerindex, data))
					{
						return false;
					}
					break;
				}
			}
		}
	}
	return true;
}

void VirtualMachineManager::FireCommandCallback(const unsigned char index, const char *callback, const unsigned char numargs)
{
	if (index >= vmbuffersize)
	{
		return;
	}
	if (vmbuffer[index] == NULL)
	{
		return;
	}
	switch (vmbuffer[index]->lang)
	{
	case VMLanguageSquirrel:
		{
			sc_CommandCallback(*vmbuffer[index]->ptr.squirrel, callback, numargs);
			break;
		}
	}
}

bool VirtualMachineManager::LoadFilterScriptInternal(const unsigned char index, const char *string)
{
	int length = strlen(string);
	char *filterscript = (char *)calloc(length + 15, sizeof(char));
	sprintf(filterscript, "filterscripts/%s", string);
	if (!this->LoadVirtualMachine(index, filterscript))
	{
		return false;
	}
	vmbuffer[index]->filename = (char *)calloc(length + 1, sizeof(char));
	strcpy(vmbuffer[index]->filename, string);
	this->OnFilterScriptInit(index);
	return true;
}

bool VirtualMachineManager::LoadVirtualMachine(const unsigned char index, const char *string)
{
	if (index >= maxvmbuffersize)
	{
		return false;
	}
	if (index >= vmbuffersize)
	{
		if (vmbuffersize == maxvmbuffersize)
		{
			return false;
		}
		if (!ResizeBuffer<VirtualMachine **>(vmbuffer, index + 1))
		{
			return false;
		}
		vmbuffer[index] = NULL;
		vmbuffersize = index + 1;
	}
	if (vmbuffer[index] != NULL)
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
	vmbuffer[index] = new VirtualMachine;
	switch (lang)
	{
	case VMLanguageSquirrel:
		{
			vmbuffer[index]->ptr.squirrel = new HSQUIRRELVM;
			// Init Squirrel
			*vmbuffer[index]->ptr.squirrel = sq_open(1024);
			sqstd_seterrorhandlers(*vmbuffer[index]->ptr.squirrel);
			sq_setprintfunc(*vmbuffer[index]->ptr.squirrel, sq_PrintToServer);
			// Register Script Funcions
			// Script identity functions
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetScriptName, "SetScriptName");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetScriptVersion, "SetScriptVersion");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetScriptAuthor, "SetScriptAuthor");
			// Console functions
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_CreateConVar, "CreateConVar");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_FindConVar, "FindConVar");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_ResetConVar, "ResetConVar");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetConVarName, "GetConVarName");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetConVarFloat, "GetConVarFloat");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetConVarInt, "GetConVarInt");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetConVarString, "GetConVarString");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetConVarFlags, "GetConVarFlags");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetConVarBoundFloat, "GetConVarBoundFloat");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetConVarBoundInt, "GetConVarBoundInt");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetConVarFloat, "SetConVarFloat");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetConVarInt, "SetConVarInt");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetConVarString, "SetConVarString");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetConVarFlags, "SetConVarFlags");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetConVarBoundFloat, "SetConVarBoundFloat");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetConVarBoundInt, "SetConVarBoundInt");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_RegServerCmd, "RegServerCmd");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgs, "GetCmdArgs");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgsAsString, "GetCmdArgsAsString");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgType, "GetCmdArgType");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgString, "GetCmdArgString");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgInt, "GetCmdArgInt");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgFloat, "GetCmdArgFloat");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_printr, "printr");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_ServerCommand, "ServerCommand");
			// Player functions
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetPlayerName, "GetPlayerName");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetPlayerModel, "GetPlayerModel");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetPlayerAngle, "GetPlayerAngle");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetPlayerScore, "GetPlayerScore");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetPlayerScore, "GetPlayerHealth");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetPlayerScore, "GetPlayerArmor");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetPlayerWantedLevel, "GetPlayerWantedLevel");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GiveWeapon, "GiveWeapon");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_addPlayerClass, "addPlayerClass");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_enableComponentSelect, "enableComponentSelect");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetPlayerSpawnPos, "SetPlayerSpawnPos");
			// Time func
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetGameTime, "SetGameTime");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetGameHour, "GetGameHour");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetGameMinutes, "GetGameMinutes");

			// Car functions
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_CreateCar, "CreateCar");
			sq_pushroottable(*vmbuffer[index]->ptr.squirrel);
			// Register Standard Script Functions
			sqstd_register_stringlib(*vmbuffer[index]->ptr.squirrel);
			sqstd_register_mathlib(*vmbuffer[index]->ptr.squirrel);
			sqstd_register_systemlib(*vmbuffer[index]->ptr.squirrel);
			sqstd_seterrorhandlers(*vmbuffer[index]->ptr.squirrel);
			if(!SQ_SUCCEEDED(sqstd_dofile(*vmbuffer[index]->ptr.squirrel, _SC(string), 0, 1))) 
			{
				sq_close(*vmbuffer[index]->ptr.squirrel);
				delete vmbuffer[index]->ptr.squirrel;
				delete vmbuffer[index];
				vmbuffer[index] = NULL;
				return false;
			}
			vmbuffer[index]->lang = VMLanguageSquirrel;
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
	vmbuffer[index]->paused = false;
	vmbuffer[index]->name = (char *)calloc(16, sizeof(char));
	strcpy(vmbuffer[index]->name, "Untitled script");
	vmbuffer[index]->version = (char *)calloc(8, sizeof(char));
	strcpy(vmbuffer[index]->version, "0.0.0.0");
	vmbuffer[index]->author = (char *)calloc(15, sizeof(char));
	strcpy(vmbuffer[index]->author, "Unnamed author");
	return true;
}

bool VirtualMachineManager::UnloadVirtualMachine(const unsigned char index)
{
	if (index >= vmbuffersize)
	{
		return false;
	}
	if (vmbuffer[index] == NULL)
	{
		return false;
	}
	switch (vmbuffer[index]->lang)
	{
	case VMLanguageSquirrel:
		{
			sq_close(*vmbuffer[index]->ptr.squirrel);
			delete vmbuffer[index]->ptr.squirrel;
			break;
		}
	}
	vmbuffer[index]->paused = false;
	free(vmbuffer[index]->name);
	free(vmbuffer[index]->version);
	free(vmbuffer[index]->author);
	hm.CloseAllHandles(index + 1);
	return true;
}

unsigned char VirtualMachineManager::GetNumberOfFreeFilterScriptSlots(void)
{
	unsigned char slots = 0;
	for (unsigned char i = 1; i < vmbuffersize; i++)
	{
		if (vmbuffer[i] == NULL)
		{
			slots++;
		}
	}
	slots = maxvmbuffersize - vmbuffersize + slots - 1;
	return slots;
}

bool VirtualMachineManager::GetFilterScriptFreeSlot(unsigned char &index)
{
	for (index = 1; index < vmbuffersize; index++)
	{
		if (vmbuffer[index] == NULL)
		{
			return true;
		}
	}
	if (vmbuffersize == maxvmbuffersize)
	{
		return false;
	}
	return true;
}

void VirtualMachineManager::OnGameModeInit(void)
{
	for (unsigned char i = 0; i < vmbuffersize; i++)
	{
		if (vmbuffer[i] != NULL)
		{
			switch (vmbuffer[i]->lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnGameModeInit(*vmbuffer[i]->ptr.squirrel);
					break;
				}
			}
		}
	}
}

void VirtualMachineManager::OnGameModeExit(void)
{
	for (unsigned char i = 0; i < vmbuffersize; i++)
	{
		if (vmbuffer[i] != NULL)
		{
			switch (vmbuffer[i]->lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnGameModeExit(*vmbuffer[i]->ptr.squirrel);
					break;
				}
			}
		}
	}
}

void VirtualMachineManager::OnFilterScriptInit(const unsigned char index)
{
	if (index >= vmbuffersize)
	{
		return;
	}
	if (vmbuffer[index] == NULL)
	{
		return;
	}
	switch (vmbuffer[index]->lang)
	{
	case VMLanguageSquirrel:
		{
			sc_OnFilterScriptInit(*vmbuffer[index]->ptr.squirrel);
			break;
		}
	}
}

void VirtualMachineManager::OnFilterScriptExit(const unsigned char index)
{
	if (index >= vmbuffersize)
	{
		return;
	}
	if (vmbuffer[index] == NULL)
	{
		return;
	}
	switch (vmbuffer[index]->lang)
	{
	case VMLanguageSquirrel:
		{
			sc_OnFilterScriptExit(*vmbuffer[index]->ptr.squirrel);
			break;
		}
	}
}