#pragma once

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
	bool SetIndex(const short i);
	unsigned int GetSessionKey(void);
private:
	char name[MAX_PLAYER_NAME_LENGTH];
	short index;
	unsigned int sessionkey;
};
