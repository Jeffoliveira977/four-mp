/*
*	Created:			29.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#pragma once
#include "../../Shared/Network/Limits.h"
#include "Hook/types.h"
#include "Hook/scripting.h"

struct FPlayer
{
	Ped PedID;
	bool connected;
	char ip[16];
	unsigned short port;
	char name[MAX_PLAYER_NAME_LENGTH];
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
	int health, armor;
	char wanted_level;
	bool edSprint, edLockon, edDoDriveBy, edUseCover, edConrol, edFreeze; // enable / disable
	bool isaiming; ///< Holds true if the player is aiming.
	bool isducking; ///< Holds true if the player is ducking.
	int room;
	unsigned char color[4];
	int compT[11], compD[11];
};

struct PlayerClass
{
	unsigned int model;
	float position[3];
	float angle;
	char gun[3];
	short ammo[3];
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
	unsigned char color[2];
};

struct FConfig
{
	char server[32];
	int port;
	char Name[MAX_PLAYER_NAME_LENGTH];

	bool ComponentSelect;
	unsigned char NumSkins;
	bool SkinSelect;
};

class COLOR
{
public:
	COLOR() {  }
	COLOR(unsigned char r, unsigned char g, unsigned char b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	unsigned char r, g, b;
};

struct CHATMSG 
{
	char msg[MAX_CHAT_MESSAGE_LENGTH];
	COLOR color;
};