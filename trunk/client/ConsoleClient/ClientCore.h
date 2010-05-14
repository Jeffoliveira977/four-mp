#pragma once

#include "../../Shared/Console/ConVar.h"
#include "../../Shared/Network/Limits.h"

class ClientCore
{
public:
	ClientCore(void);
	~ClientCore(void);
	bool Load(void);
	bool IsRunning(void);
	void Tick(void);
	void Unload(void);
	void Shutdown(void);
	char *GetName(void);
	short GetIndex(void);
	bool SetIndex(short i);
private:
	bool isrunning;
	char name[MAX_PLAYER_NAME_LENGTH];
	short index;
	friend void ConVarHookName(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
};
