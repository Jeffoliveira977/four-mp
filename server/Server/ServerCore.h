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
	void UpdateServerInfo(void);
	bool IsLAN(void);
	bool IsPasswordProtected(void);
	wchar_t *GetHostname(void);
	bool GetComponentSelectStatus(void);
	void EnableComponentSelect(bool enable);
	void GetTime(unsigned char (&timebuffer)[2]);
	void SetTime(const unsigned char time[2]);
private:
	bool isrunning;
	short sleepcount;
	unsigned int lastcheck;
	unsigned int lastmasterservercheck;
	wchar_t *hostname;
	unsigned short port;
	bool lan;
	short maxplayers;
	wchar_t *gamemode;
	wchar_t *gamemodename;
	wchar_t *password;
	wchar_t *rconpassword;
	bool componentselect;
	ConVar *componentselectcvar;
	struct GameTime
	{
		short ticksperminute;
		short tickcount;
		unsigned char time[2];
	};
	GameTime gametime;
	void IncrementGameTime(void);
	void UpdateCaption(void);
	friend void ConVarHookHostGamemode(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookHostname(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookRconPassword(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookSvComponentselect(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookSvLan(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookSvPassword(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
	friend void ConVarHookSvPort(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
};
