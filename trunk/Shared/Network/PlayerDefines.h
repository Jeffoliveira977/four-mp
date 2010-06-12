#pragma once

#define INVALID_PLAYER_INDEX -1
#define MAX_PLAYER_INDEX 65532
#define MAX_PLAYERS 32 //Temp.
#define INVALID_PLAYER_MODEL 0
#define DEFAULT_PLAYER_MODEL 0x98E29920
#define DEFAULT_PLAYER_X -43.250000
#define DEFAULT_PLAYER_Y -2.875000
#define DEFAULT_PLAYER_Z 14.818866

enum PlayerState
{
	PlayerStateOnFoot,
	PlayerStateEnteringVehicle,
	PlayerStateInVehicle,
	PlayerStateExitingVehicle
};

#define INVALID_PLAYER_SEAT_INDEX -1
#define INVALID_PLAYER_WANTED_LEVEL -1