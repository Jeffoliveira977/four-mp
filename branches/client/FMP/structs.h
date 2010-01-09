/*
*	Created:			29.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#pragma once
#include "Hook/types.h"

struct FPlayer
{
	Ped PedID;
	bool connected;
	char ip[32];
	int port;
	char name[32];
	int model;
	float x,y,z;
	float angle;
	int last_active;
	int sync_state;
	int gun;
	int gWeapons[8];
	int gAmmo[8];
	char animation[128];
	int car_id;
	int seat_id;
	int score;
	int health, armour;
	int wanted_level;
	bool edSprint, edLockon, edDoDriveBy, edUseCover, edConrol, edFreeze; // enable / disable
	bool Aim;
	bool duck;
	int Room;
	char color[4];
	int CompT[11], CompD[11];
};

struct PlayerDamage
{
	int pid;
	int hp;
	int armour;
};

struct PlayerClass
{
	int model;
	float x, y, z;
	float angle;
	int gun[3], ammo[3];
};

struct SpawnInfo
{
	int model;
	float x, y, z, r;
	int room;
	int CompT[11], CompD[11];
	int health, armour;
};

struct FVehicle
{
	Vehicle CarID;
	bool exist;
	int model;
	float x,y,z;
	float angle;
	int Health, engHealth;
	int DoorState[6];
	int DoorLock[6];
	int color[2];
};

struct FConfig
{
	char server[32];
	int port;
	char Name[32];

	bool ComponentSelect;
	int NumSkins;
	bool SkinSelect;
};
