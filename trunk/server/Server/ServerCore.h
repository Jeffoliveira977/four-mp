#pragma once

#include "../../Shared/Console/ConVar.h"

class ServerCore
{
public:
	ServerCore(void);
	~ServerCore(void);
	bool Load(void);
	bool IsRunning(void);
	void Tick(void);
	void Unload(void);
	void Shutdown(void);
	bool IsLAN(void);
	bool IsPasswordProtected(void);
	char *GetHostname(void);
	bool GetComponentSelectStatus(void);
	void EnableComponentSelect(bool enable);
private:
	bool running;
	unsigned int lastcheck;
	unsigned int lastmasterservercheck;
	char *hostname;
	unsigned short port;
	bool lan;
	short maxplayers;
	char *gamemode;
	char *gamemodename;
	char *password;
	char *rconpassword;
	bool componentselect;
	ConVar *componentselectcvar;
	friend void ConVarHookHostGamemode(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookHostname(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookRconPassword(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookSvComponentselect(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookSvLan(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookSvPassword(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookSvPort(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
};
