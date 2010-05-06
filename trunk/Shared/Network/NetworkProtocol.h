#pragma once

#define PROTOCOL_VERSION 1

struct NetworkPlayerConnectionRequestData
{
	char name[32];
};

enum NetworkPlayerConnectionError
{
	NetworkPlayerConnectionErrorServerFull,
	NetworkPlayerConnectionErrorAlreadyConnected,
	NetworkPlayerConnectionErrorAllocationError,
	NetworkPlayerConnectionErrorScriptLock
};

struct NetworkPlayerConnectionErrorData
{
	NetworkPlayerConnectionError error;
};

struct NetworkPlayerDisconnectionData
{
	short client;
};

struct NetworkPlayerCheckData
{
	short client;
};

struct NetworkPlayerFullUpdateData
{
	short index;
	char name[32];
	int model;
	float position[3];
	float angle;
	short vehicleindex;
	int seat_id;
	int score;
	int health;
	int armor;
	int room;
	int weapons[8];
	int ammo[8];
	unsigned char color[4];
};

struct NetworkPlayerMoveData
{
	short client;
	float position[3];
	float angle;
	float speed;
};

struct NetworkPlayerJumpData
{
	short client;
};

struct NetworkPlayerDuckData
{
	short client;
	bool shouldduck;
};

struct NetworkPlayerEntranceInVehicleData
{
	short client;
	short vehicleindex;
	int seat;
};

struct NetworkPlayerCancelEntranceInVehicleData
{
	short client;
};

struct NetworkPlayerExitFromVehicleData
{
	short client;
};

struct NetworkPlayerFireData
{
	short client;
	float position[3];
	int weapon;
	int time;
	short target;
	unsigned int health;
	unsigned int armor;
};

struct NetworkPlayerAimData
{
	short client;
	float position[3];
	int weapon;
	int time;
};

struct NetworkPlayerWeaponChangeData
{
	short client;
	int weapon;
};

struct NetworkPlayerHealthAndArmorChangeData
{
	short client;
	int health;
	int armor;
};

struct NetworkPlayerSpawnRequestData
{
	unsigned char playerclassindex;
};

struct NetworkPlayerSpawnData
{
	short client;
	int model;
	float position[3];
	float angle;
	int room;
	int compT[11];
	int compD[11];
	int health;
	int armor;
};

struct NetworkPlayerModelChangeData
{
	short client;
	int model;
};

struct NetworkPlayerComponentsChangeData
{
	short client;
	int compD[11];
	int compT[11];
};

struct NetworkPlayerChatData
{
	short client;
	char msg[128];
	unsigned char color[4];
};

struct NetworkVehicleFullUpdateData
{
	short index;
	int model;
	float position[3];
	float angle;
	unsigned char color[2];
};