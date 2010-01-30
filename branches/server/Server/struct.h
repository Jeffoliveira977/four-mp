/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#ifndef STRUCTS
#include "net\RakPeerInterface.h"

struct Player
{
	bool connected;
	char ip[32];
	int port;
	SystemAddress sa;
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
	bool duck;
	int Room;
	bool car_enter;
	char color[4];
	int CompT[11], CompD[11];

	void Clean()
	{
		ip[0] = 0;
		port = 0;
		name[0] = 0;
		model = 0x98E29920;
		x = 0;
		y = 0;
		z = 0;
		angle = 0;
		last_active = 0;
		sync_state = 0;
		for(int i=0; i<8; i++)
		{
			gWeapons[i] = 0;
			gAmmo[i] = 0;
		}
		animation[0] = 0;
		car_id = -1;
		seat_id = 99;
		score = 0;
		health = 200;
		armour = 0;
		wanted_level = 0;
		edSprint = 1;
		edLockon = 1;
		edDoDriveBy = 1;
		edUseCover = 1;
		edConrol = 1;
		edFreeze = 0;
		duck = 0;
		Room = 0;
		car_enter = 0;
		color[0] = 0xFF;
		color[1] = 0xFF;
		color[2] = 0x00;
		color[3] = 0x00;
		gun = 0;
	}
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

struct Vehicle
{
	bool exist;
	int model;
	float x,y,z;
	float angle;
	int Health, engHealth;
	int DoorState[6];
	int DoorLock[6];
	int color[2];

};

struct Server
{
	bool Run;
	int Port;
	char GameMode[4][64];
	char FilterScripts[512];
	char Name[128];
	char Lang[32];
	char Password[32];
	char RconPassword[32];
	char ServerURL[128];

	bool ComponentSelect;
	int NumSkins;
};

struct SpawnInfo
{
	int model;
	float x, y, z, r;
	int room;
	int CompT[11], CompD[11];
	int health, armour;
};

#define STRUCTS
#endif