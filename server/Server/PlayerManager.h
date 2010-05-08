#pragma once

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
	int GetPlayerModel(const short index);
	bool GetPlayerPosition(const short index, float (&postion)[3]);
	bool GetPlayerAngle(const short index, float &angle);
	short GetPlayerVehicle(const short index);
	bool GetPlayerScore(const short index, int &score);
	bool GetPlayerHealth(const short index, int &health);
	bool GetPlayerArmor(const short index, int &armor);
	char GetPlayerWantedLevel(const short index);
	bool GetPlayerColor(const short index, unsigned char (&color)[4]);
	unsigned char GetNumberOfPlayerClasses(void);
	bool AddPlayerClass(const int model, const float position[3], const float angle, const int weapons[8], const int ammo[8], unsigned char &index);
	bool GetPlayerClassData(const unsigned char index, int &model, float (&position)[3], float &angle, int (&weapons)[8], int (&ammo)[8]);
private:
	struct Player
	{
		char name[32]; ///< Holds the name of the player.
		unsigned int model; ///< Holds the model hash of the player.
		float position[3]; ///< Holds the position of the player.
		float angle; ///< Holds the angle of the player.
		int last_active;
		int sync_state;
		char currentweapon;
		char weapons[8];
		short ammo[8];
		char animation[128];
		short vehicleindex; ///< Holds
		char seatindex;
		int score;
		int health, armor; // TODO: test and reduce
		char wanted_level;
		bool edSprint, edLockon, edDoDriveBy, edUseCover, edConrol, edFreeze; // enable / disable
		bool isducking;
		int room;
		bool car_enter;
		unsigned char color[4];
		int compT[11], compD[11];
	};
	short maxplayerbuffersize;
	short playerbuffersize;
	short numplayers;
	Player **playerbuffer;
	struct PlayerClass
	{
		int model;
		float position[3];
		float angle;
		int weapons[8], ammo[8];
	};
	unsigned char maxclassbuffersize;
	unsigned char classbuffersize;
	PlayerClass **classbuffer;
	bool RegisterNewPlayer(const short index, const char *name);
	short GetPlayerFreeSlot(void);
	bool GetClassFreeSlot(unsigned char &index);
	friend class NetworkManager;
};
