#pragma once

#include "../../Shared/Console/ConVar.h"

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
private:
	bool isrunning;
	char name[32];
	friend void ConVarHookName(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
};
