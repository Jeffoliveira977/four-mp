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
	maxfilterscripts = 16;
	vmbuffer = (VirtualMachine **)calloc(1, sizeof(VirtualMachine *));
	maxfilterscriptindex = 0;
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

unsigned char VirtualMachineManager::GetMaxFilterScripts(void)
{
	return maxfilterscripts;
}

unsigned char VirtualMachineManager::GetMaxFilterScriptIndex(void)
{
	return maxfilterscriptindex;
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
	for (unsigned char i = 1; i <= maxfilterscriptindex; i++)
	{
		if (vmbuffer[i] != NULL)
		{
			this->UnloadFilterScript(i);
		}
	}
}

void VirtualMachineManager::ReloadFilterScripts(void)
{
	for (unsigned char i = 1; i <= maxfilterscriptindex; i++)
	{
		if (vmbuffer[i] != NULL)
		{
			this->ReloadFilterScript(i);
		}
	}
}

void VirtualMachineManager::PauseVirtualMachines(void)
{
	for (unsigned char i = 0; i <= maxfilterscriptindex; i++)
	{
		if (vmbuffer[i] != NULL)
		{
			this->PauseVirtualMachine(i);
		}
	}
}

void VirtualMachineManager::UnpauseVirtualMachines(void)
{
	for (unsigned char i = 0; i <= maxfilterscriptindex; i++)
	{
		if (vmbuffer[i] != NULL)
		{
			this->UnpauseVirtualMachine(i);
		}
	}
}

bool VirtualMachineManager::IsFilterScriptLoaded(const char *string)
{
	for (unsigned char i = 1; i <= maxfilterscriptindex; i++)
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
	if (index > maxfilterscriptindex)
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
	if (index > maxfilterscriptindex)
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
	if (index > maxfilterscriptindex)
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
	if (index > maxfilterscriptindex)
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

bool VirtualMachineManager::GetVirtualMachineInfoString(const unsigned char index, char *&string)
{
	if (index > maxfilterscriptindex)
	{
		return false;
	}
	if (vmbuffer[index] == NULL)
	{
		return false;
	}
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
	return true;
}

void VirtualMachineManager::SetVirtualMachineName(const HSQUIRRELVM *v, const char *string)
{
	unsigned char index;
	if (!this->FindVirtualMachine(v, index))
	{
		return;
	}
	ResizeStringBuffer(vmbuffer[index]->name, strlen(string) + 1);
	strcpy(vmbuffer[index]->name, string);
}

void VirtualMachineManager::SetVirtualMachineVersion(const HSQUIRRELVM *v, const char *string)
{
	unsigned char index;
	if (!this->FindVirtualMachine(v, index))
	{
		return;
	}
	ResizeStringBuffer(vmbuffer[index]->version, strlen(string) + 1);
	strcpy(vmbuffer[index]->version, string);
}

void VirtualMachineManager::SetVirtualMachineAuthor(const HSQUIRRELVM *v, const char *string)
{
	unsigned char index;
	if (!this->FindVirtualMachine(v, index))
	{
		return;
	}
	ResizeStringBuffer(vmbuffer[index]->author, strlen(string) + 1);
	strcpy(vmbuffer[index]->author, string);
}

int VirtualMachineManager::OnPlayerConnect(int playerid, char name[32])
{
	for (unsigned char i = 0; i <= maxfilterscriptindex; i++)
	{
		if ((vmbuffer[i] != NULL) && (!vmbuffer[i]->paused))
		{
			switch (vmbuffer[i]->lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnPlayerConnect(*vmbuffer[i]->ptr.squirrel, playerid, name);
					break;
				}
			}
		}
	}
	return 1; //Temp
}

void VirtualMachineManager::OnPlayerDisconnect(int playerid)
{
	for (unsigned char i = 0; i <= maxfilterscriptindex; i++)
	{
		if ((vmbuffer[i] != NULL) && (!vmbuffer[i]->paused))
		{
			switch (vmbuffer[i]->lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnPlayerDisconnect(*vmbuffer[i]->ptr.squirrel, playerid);
					break;
				}
			}
		}
	}
}

void VirtualMachineManager::OnPlayerSpawn(int playerid, int cl)
{
	for (unsigned char i = 0; i <= maxfilterscriptindex; i++)
	{
		if ((vmbuffer[i] != NULL) && (!vmbuffer[i]->paused))
		{
			switch (vmbuffer[i]->lang)
			{
			case VMLanguageSquirrel:
				{
					sc_OnPlayerSpawn(*vmbuffer[i]->ptr.squirrel, playerid, cl);
					break;
				}
			}
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
	if (index > maxfilterscripts)
	{
		return false;
	}
	if (index > maxfilterscriptindex)
	{
		if (maxfilterscriptindex == maxfilterscripts)
		{
			return false;
		}
		if (!this->ResizeVirtualMachineBuffer(vmbuffer, index + 1))
		{
			return false;
		}
		vmbuffer[index] = NULL;
		maxfilterscriptindex = index;
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
			sq_setprintfunc(*vmbuffer[index]->ptr.squirrel, printfunc);
			// Register Script Funcions
			// Script identity functions
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetScriptName, "SetScriptName");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetScriptVersion, "SetScriptVersion");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_SetScriptAuthor, "SetScriptAuthor");
			// Console functions
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgs, "GetCmdArgs");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgsAsString, "GetCmdArgsAsString");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgType, "GetCmdArgType");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgString, "GetCmdArgString");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgInt, "GetCmdArgInt");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GetCmdArgFloat, "GetCmdArgFloat");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_printr, "printr");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_RegServerCmd, "RegServerCmd");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_ServerCommand, "ServerCommand");
			// Car functions
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_CreateCar, "CreateCar");
			// Player functions
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_GiveWeapon, "GiveWeapon");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_addPlayerClass, "addPlayerClass");
			register_global_func(*vmbuffer[index]->ptr.squirrel, (SQFUNCTION)sq_enableComponentSelect, "enableComponentSelect");
			sq_pushroottable(*vmbuffer[index]->ptr.squirrel);
			// Register Standart Script Functions
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
	if (index > maxfilterscriptindex)
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
	return true;
}

unsigned char VirtualMachineManager::GetNumberOfFreeFilterScriptSlots(void)
{
	unsigned char slots = 0;
	for (unsigned char i = 1; i <= maxfilterscriptindex; i++)
	{
		if (vmbuffer[i] == NULL)
		{
			slots++;
		}
	}
	slots = maxfilterscripts - maxfilterscriptindex + slots;
	return slots;
}

bool VirtualMachineManager::GetFilterScriptFreeSlot(unsigned char &index)
{
	for (index = 1; index <= maxfilterscriptindex; index++)
	{
		if (vmbuffer[index] == NULL)
		{
			return true;
		}
	}
	if (maxfilterscriptindex == maxfilterscripts)
	{
		return false;
	}
	return true;
}

bool VirtualMachineManager::FindVirtualMachine(const HSQUIRRELVM *v, unsigned char &index)
{
	for (index = 0; index <= maxfilterscriptindex; index++)
	{
		if ((vmbuffer[index] != NULL) && (vmbuffer[index]->lang == VMLanguageSquirrel) && (*vmbuffer[index]->ptr.squirrel == *v))
		{
			return true;
		}
	}
	return false;
}

bool VirtualMachineManager::ResizeVirtualMachineBuffer(VirtualMachine **&buffer, const unsigned char size)
{
	VirtualMachine **tempbuffer = (VirtualMachine **)realloc(*&buffer, size * sizeof(VirtualMachine *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

void VirtualMachineManager::OnGameModeInit(void)
{
	for (unsigned char i = 0; i <= maxfilterscriptindex; i++)
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
	for (unsigned char i = 0; i <= maxfilterscriptindex; i++)
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
	if (index > maxfilterscriptindex)
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
	if (index > maxfilterscriptindex)
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