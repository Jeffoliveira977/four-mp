/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/
#ifndef _MANAGER_
#include "struct.h"

int GetPlayerID(SystemAddress ad);
int GetPlayerID(const char *ip, int port);
int GetFreePlayerClass();

int Man_PlayerConnect(char *name, int index);
void Man_PlayerDisconnect(int index);
void Man_PlayerSpawn(int, int,SpawnInfo*);

#define _MANAGER_
#endif