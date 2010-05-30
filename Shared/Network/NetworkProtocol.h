#pragma once

#include "Limits.h"

#define PROTOCOL_VERSION 6

#define DEFAULT_SERVER_NETWORK_ID 0
#define DEFAULT_CLIENT_NETWORK_ID 65534

struct NetworkPlayerConnectionRequestData
{
	short protocol;
	char name[MAX_PLAYER_NAME_LENGTH];
	unsigned int sessionkey;
};

enum NetworkPlayerConnectionError
{
	NetworkPlayerConnectionErrorServerFull,
	NetworkPlayerConnectionErrorInvalidProtocol,
	NetworkPlayerConnectionErrorInvalidName,
	NetworkPlayerConnectionErrorAlreadyConnected,
	NetworkPlayerConnectionErrorAllocationError,
	NetworkPlayerConnectionErrorScriptLock
};

struct NetworkPlayerConnectionErrorData
{
	NetworkPlayerConnectionError error;
};

struct NetworkPlayerInfoData
{
	short index;
	unsigned int sessionkey;
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
	char name[MAX_PLAYER_NAME_LENGTH];
	unsigned int model;
	float position[3];
	float angle;
	short vehicleindex;
	char seatindex;
	int score;
	int health;
	int armor;
	int room;
	char weapons[8];
	short ammo[8];
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
	char seat;
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
	char weapon;
	int time;
	short target;
	unsigned int health;
	unsigned int armor;
};

struct NetworkPlayerAimData
{
	short client;
	float position[3];
	char weapon;
	int time;
};

struct NetworkPlayerWeaponChangeData
{
	short client;
	char weapon;
};

struct NetworkPlayerHealthAndArmorChangeData
{
	short client;
	int health;
	int armor;
};

struct NetworkPlayerSpawnRequestData
{
	short client;
};

struct NetworkPlayerSpawnData
{
	short client;
	unsigned int model;
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
	unsigned int model;
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
	char msg[MAX_CHAT_MESSAGE_LENGTH];
	unsigned char color[4];
};

struct NetworkVehicleFullUpdateData
{
	short index;
	unsigned int model;
	float position[3];
	float angle;
	unsigned char color[2];
};

struct NetworkTimeData
{
	int hour, minute;
};