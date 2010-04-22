#pragma once

#include "console\ConVar.h"

class ServerCore
{
public:
	ServerCore(void);
	~ServerCore(void);
	bool Load(void);
	bool IsRunning(void);
	void Tick(void);
	bool GetComponentSelectStatus(void);
	void EnableComponentSelect(bool enable);
private:
	bool running;
	unsigned int lastcheck;
	ConVar *hostname;
	ConVar *gamemode;
	ConVar *password;
	ConVar *rconpassword;
	ConVar *componentselect;
};
