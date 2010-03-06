#pragma once

#include "sq\squirrel.h"

enum VirtualMachineLanguage
	{
		VMLanguageSquirrel
		//VMLanguagePawn
		//VMLanguageLua
	};

class VirtualMachineManager
{
public:
	VirtualMachineManager(void);
	~VirtualMachineManager(void);
	bool IsGameModeLoaded(void);
	bool LoadGameMode(const char *string);
	bool UnloadGameMode(void);
	unsigned char GetMaxFilterScripts(void);
	void LoadFilterScripts(void);
	void UnloadFilterScripts(void);
	void ReloadFilterScripts(void);
	void PauseVirtualMachines(void);
	void UnpauseVirtualMachines(void);
	bool IsFilterScriptLoaded(const char *string);
	bool LoadFilterScript(const char *string);
	bool UnloadFilterScript(const unsigned char index);
	bool ReloadFilterScript(const unsigned char index);
	bool PauseVirtualMachine(const unsigned char index);
	bool UnpauseVirtualMachine(const unsigned char index);
	bool GetVirtualMachineInfoString(const unsigned char index, char *&string);
	void SetVirtualMachineName(HSQUIRRELVM *v, const char *string);
	void SetVirtualMachineVersion(HSQUIRRELVM *v, const char *string);
	void SetVirtualMachineAuthor(HSQUIRRELVM *v, const char *string);
	int OnPlayerConnect(int playerid, char name[32]);
	void OnPlayerDisconnect(int playerid);
	void OnPlayerSpawn(int playerid, int cl);
private:
	union VirtualMachinePtr
	{
		HSQUIRRELVM *squirrel;
		//AMX *amx;
		//Lua
	};
	struct VirtualMachine
	{
		bool paused;
		char *name;
		char *version;
		char *author;
		VirtualMachineLanguage lang;
		VirtualMachinePtr ptr;
		char *filename;
	};
	unsigned char maxfilterscripts;
	VirtualMachine **vmbuffer; //Virtual machine buffer. 0 - is game mode, 1...maxfilterscripts - filterscripts
	bool LoadFilterScriptInternal(const unsigned char index, const char *string);
	bool LoadVirtualMachine(const unsigned char index, const char *string);
	bool UnloadVirtualMachine(const unsigned char index);
	unsigned char GetNumberOfFreeFilterScriptSlots(void);
	bool GetFilterScriptFreeSlot(unsigned char &index);
	bool FindVirtualMachine(HSQUIRRELVM *v, unsigned char &index);
	void OnGameModeInit(void);
	void OnGameModeExit(void);
	void OnFilterScriptInit(const unsigned char index);
	void OnFilterScriptExit(const unsigned char index);
};
