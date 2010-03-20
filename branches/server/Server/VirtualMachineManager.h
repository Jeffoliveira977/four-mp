#pragma once

#include "sq\squirrel.h"
#include "console\ConsoleCore.h"

class VirtualMachineManager
{
public:
	VirtualMachineManager(void);
	~VirtualMachineManager(void);
	bool IsGameModeLoaded(void);
	bool LoadGameMode(const char *string);
	bool UnloadGameMode(void);
	unsigned char GetMaxFilterScripts(void);
	unsigned char GetMaxFilterScriptIndex(void);
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
	void SetVirtualMachineName(const HSQUIRRELVM *v, const char *string);
	void SetVirtualMachineVersion(const HSQUIRRELVM *v, const char *string);
	void SetVirtualMachineAuthor(const HSQUIRRELVM *v, const char *string);
	int CreateConVar(const HSQUIRRELVM *v, const ConVar *ptr);
	int FindConVar(const HSQUIRRELVM *v, const char *name);
	bool ResetConVar(const HSQUIRRELVM *v, const int handle);
	char *GetConVarName(const HSQUIRRELVM *v, const int handle);
	bool GetConVarValue(const HSQUIRRELVM *v, const int handle, float &value);
	bool GetConVarValue(const HSQUIRRELVM *v, const int handle, int &value);
	bool GetConVarValue(const HSQUIRRELVM *v, const int handle, char *&value);
	bool GetConVarFlags(const HSQUIRRELVM *v, const int handle, int &flags);
	bool GetConVarBound(const HSQUIRRELVM *v, const int handle, const ConVarBoundType type, float &bound);
	bool GetConVarBound(const HSQUIRRELVM *v, const int handle, const ConVarBoundType type, int &bound);
	bool SetConVarValue(const HSQUIRRELVM *v, const int handle, const float value);
	bool SetConVarValue(const HSQUIRRELVM *v, const int handle, const int value);
	bool SetConVarValue(const HSQUIRRELVM *v, const int handle, const char *value);
	bool SetConVarFlags(const HSQUIRRELVM *v, const int handle, const int flags);
	bool SetConVarBound(const HSQUIRRELVM *v, const int handle, const ConVarBoundType, const float bound);
	bool SetConVarBound(const HSQUIRRELVM *v, const int handle, const ConVarBoundType, const int bound);
	void RegServerCmd(const HSQUIRRELVM *v, const char *callback, const ConCmd *ptr);
	int OnPlayerConnect(int playerid, char name[32]);
	void OnPlayerDisconnect(int playerid);
	void OnPlayerSpawn(int playerid, int cl);
	void FireCommandCallback(const unsigned char index, const char *callback, const unsigned char numargs);
private:
	enum VirtualMachineLanguage
	{
		VMLanguageSquirrel
		//VMLanguagePawn
		//VMLanguageLua
	};
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
	unsigned char maxfilterscriptindex;
	VirtualMachine **vmbuffer; //Virtual machine buffer. 0 - is game mode, 1...maxfilterscripts - filterscripts
	bool LoadFilterScriptInternal(const unsigned char index, const char *string);
	bool LoadVirtualMachine(const unsigned char index, const char *string);
	bool UnloadVirtualMachine(const unsigned char index);
	unsigned char GetNumberOfFreeFilterScriptSlots(void);
	bool GetFilterScriptFreeSlot(unsigned char &index);
	bool FindVirtualMachine(const HSQUIRRELVM *v, unsigned char &index);
	bool ResizeVirtualMachineBuffer(VirtualMachine **&buffer, const unsigned char size);
	void OnGameModeInit(void);
	void OnGameModeExit(void);
	void OnFilterScriptInit(const unsigned char index);
	void OnFilterScriptExit(const unsigned char index);
};
