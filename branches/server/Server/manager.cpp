/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/
#include <windows.h>

#include "main.h"
#include "sq.h"
#include "manager.h"

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

int Man_PlayerConnect(char *name, int index)
{
	gPlayer[index].Clean();
	
	debug("### I'm set PLAYER MODEL");
	if(sc_OnPlayerConnect(v, index, name) == 0)
	{
		return 3;
	}
	return 0;
}

void Man_PlayerDisconnect(int index)
{
	if(index == -1)
		return;

	gPlayer[index].connected = 0;

	sc_OnPlayerDisconnect(v, index);
}

void Man_PlayerSpawn(int pl, int cl, SpawnInfo *sp)
{
	sp->armour = 0;
	sp->health = 200;
	sp->model = pClass[cl].model;
	sp->r = pClass[cl].angle;
	sp->room = 0;
	sp->x = pClass[cl].x;
	sp->y = pClass[cl].y;
	sp->z = pClass[cl].z;
	for(int i=0; i<11; i++)
	{
		sp->CompD[i] = gPlayer[i].CompD[i];
		sp->CompT[i] = gPlayer[i].CompT[i];
	}
	sc_OnPlayerSpawn(v, pl, cl);
}