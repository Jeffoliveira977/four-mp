/*
*	Created:			29.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#pragma once
#include "../../Shared/Network/Limits.h"
#include "Hook/types.h"
//#include "Hook/scripting.h"

struct FPlayer
{
	Ped PedID;
	bool connected;
	char ip[16];
	unsigned short port;
	wchar_t name[MAX_PLAYER_NAME_LENGTH];
	unsigned int model;
	float position[3];
	float angle;
	int last_active;
	int sync_state;
	char currentweapon;
	char weapons[8];
	short ammo[8];
	char animation[128];
	short vehicleindex;
	char seatindex;
	int score;
	unsigned char health;
	int armor;
	char wanted_level;
	bool edSprint, edLockon, edDoDriveBy, edUseCover, edConrol, edFreeze; // enable / disable
	bool isaiming; ///< Holds true if the player is aiming.
	bool isducking; ///< Holds true if the player is ducking.
	int room;
	unsigned char color[4];
	int compT[11], compD[11];
	float spawn_pos[4];
	bool want_spawn;
};

struct FVehicle
{
	Vehicle CarID;
	bool exist;
	unsigned int model;
	float position[3];
	float angle;
	short health;
	short enginehealth;
	bool doorexists[6];
	bool doorlock[6];
	float doorangle[6];
	unsigned char color[4];
	bool sirenon;
};