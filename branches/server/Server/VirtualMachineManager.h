#pragma once

#include "sq\squirrel.h"

#define MAX_FILTERSCRIPTS 16

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
	void LoadFilterScripts(void);
	void UnloadFilterScripts(void);
	bool LoadFilterScript(const char *string);
	bool UnloadFilterScript(const unsigned char index);
	bool GetFilterScriptInfoString(const unsigned char index, char *&string);
	void SetFilterScriptName(HSQUIRRELVM *v, const char *string);
	void SetFilterScriptVersion(HSQUIRRELVM *v, const char *string);
	void SetFilterScriptAuthor(HSQUIRRELVM *v, const char *string);
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
		bool loaded;
		char *name;
		char *version;
		char *author;
		VirtualMachineLanguage lang;
		VirtualMachinePtr ptr;
	};
	VirtualMachine vmbuffer[MAX_FILTERSCRIPTS+1]; //Virtual machine buffer. 0 - is game mode, 1...16 - filterscripts
	bool LoadFilterScriptInternal(const unsigned char index, const char *string);
	bool UnloadFilterScriptInternal(const unsigned char index);
	unsigned char GetNumberOfFreeFilterscriptSlots(void);
	bool GetFilterScriptFreeSlot(unsigned char &index);
	bool FindFilterScript(HSQUIRRELVM *v, unsigned char &index);
	void OnGameModeInit(void);
	void OnGameModeExit(void);
	void OnFilterScriptInit(const unsigned char index);
	void OnFilterScriptExit(const unsigned char index);
};
