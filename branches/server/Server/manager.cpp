/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/
#include <windows.h>

#include "main.h"
#include "sq.h"

extern HSQUIRRELVM v;

int GetPlayerID(const char *ip, int port)
{
	for(int i=0; i<MAX_PLAYERS; i++)
	{
		if(strcmp(ip, gPlayer[i].name) == 0 && port == gPlayer[i].port)
			return i;
	}
	return -1;
}

int GetPlayerID(SystemAddress ad)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(ad == gPlayer[i].sa && gPlayer[i].connected == 1)
		{
			return i;
		}
	}
	return -1;
}

int PlayerConnect(char *name, int index)
{
	gPlayer[index].Clean();
	
	debug("### I'm set PLAYER MODEL");
	if(sc_OnPlayerConnect(v, index, name) == 0)
	{
		return 3;
	}
	return 0;
}

void PlayerDisconnect(int index)
{
	if(index == -1)
		return;

	gPlayer[index].connected = 0;

	sc_OnPlayerDisconnect(v, index);
}

int GetFreePlayerClass()
{
	for(int i = 0; i < MAX_PCLASS; i++)
	{
		if(pClass[i].free == 1)
			return i;
	}
	return -1;
}