#pragma once

#include "../../Shared/Network/Limits.h"

#define INVALID_PLAYER_INDEX -1
#define INVALID_PLAYER_MODEL 0
#define INVALID_PLAYER_SEAT_INDEX -1
#define INVALID_PLAYER_WANTED_LEVEL -1

class PlayerManager
{
public:
	PlayerManager(void);
	~PlayerManager(void);
	short GetMaxPlayers(void);
	short GetNumberOfPlayers(void);
	bool IsServerFull(void);
	bool IsPlayerConnected(const short index);
	char *GetPlayerName(const short index);
	unsigned int GetPlayerModel(const short index);
	bool GetPlayerPosition(const short index, float (&postion)[3]);
	bool GetPlayerAngle(const short index, float &angle);
	short GetPlayerVehicle(const short index);
	bool GetPlayerScore(const short index, int &score);
	bool GetPlayerHealth(const short index, int &health);
	bool GetPlayerArmor(const short index, int &armor);
	char GetPlayerWantedLevel(const short index);
	bool GetPlayerColor(const short index, unsigned char (&color)[4]);
	unsigned char GetNumberOfPlayerClasses(void);
	bool AddPlayerClass(const unsigned int model, const float position[3], const float angle, const char weapons[8], const short ammo[8], unsigned char &index);
	bool GetPlayerClassData(const unsigned char index, unsigned int &model, float (&position)[3], float &angle, char (&weapons)[8], short (&ammo)[8]);
	void SetPlayerSpawnPos(const short index, const float pos[4]);
private:
	struct Player
	{
		char name[MAX_PLAYER_NAME_LENGTH]; ///< Holds the name of the player.
		unsigned char classindex; ///< Holds the index of the player's class.
		unsigned int model; ///< Holds the model hash of the player.
		float position[3]; ///< Holds the position of the player.
		float angle; ///< Holds the angle of the player.
		int last_active;
		int sync_state;
		char currentweapon;
		char weapons[8];
		short ammo[8];
		char animation[128];
		short vehicleindex; ///< Holds the index of the vehicle the player is in.
		char seatindex; ///< Holds the index of the seat of the vehicle the player is in.
		int score; /// Holds the score of the player.
		int health, armor; // TODO: test and reduce
		char wanted_level; ///< Holds the wanted level of the player.
		bool edSprint, edLockon, edDoDriveBy, edUseCover, edConrol, edFreeze; // enable / disable
		bool isducking; ///< Holds true if the player is ducking and false otherwise.
		int room;
		bool car_enter;
		unsigned char color[4]; ///< Holds the color of the player.
		int compT[11], compD[11];
		float spawn_pos[4];
	};
	short maxplayerbuffersize;
	short playerbuffersize;
	short numplayers;
	Player **playerbuffer;
	struct PlayerClass
	{
		unsigned int model;
		float position[3];
		float angle;
		char weapons[8];
		short ammo[8];
	};
	unsigned char maxclassbuffersize;
	unsigned char classbuffersize;
	PlayerClass **classbuffer;
	bool RegisterNewPlayer(const short index, char (&name)[MAX_PLAYER_NAME_LENGTH]);
	short GetPlayerFreeSlot(void);
	bool AssignPlayerName(char (&name)[MAX_PLAYER_NAME_LENGTH]);
	bool GetClassFreeSlot(unsigned char &index);
	friend class NetworkManager;
};
