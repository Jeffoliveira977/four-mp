#pragma once

#define INVALID_PLAYER_INDEX -1

class PlayerManager
{
public:
	PlayerManager(void);
	~PlayerManager(void);
	short GetMaxPlayers(void);
	bool IsServerFull(void);
	bool IsPlayerConnected(const short index);
	char *GetPlayerName(const short index);
	bool GetPlayerColor(const short index, unsigned char (&color)[4]);
	unsigned char GetNumberOfPlayerClasses(void);
	bool AddPlayerClass(const int model, const float position[3], const float angle, const int weapons[8], const int ammo[8], unsigned char &index);
	bool GetPlayerClassData(const unsigned char index, int &model, float (&position)[3], float &angle, int (&weapons)[8], int (&ammo)[8]);
private:
	struct Player
	{
		char name[32];
		int model;
		float position[3];
		float angle;
		int last_active;
		int sync_state;
		int currentweapon;
		int weapons[8];
		int ammo[8];
		char animation[128];
		short vehicleindex;
		int seat_id;
		int score;
		int health, armor;
		int wanted_level;
		bool edSprint, edLockon, edDoDriveBy, edUseCover, edConrol, edFreeze; // enable / disable
		bool isducking;
		int room;
		bool car_enter;
		unsigned char color[4];
		int compT[11], compD[11];
	};
	short maxplayerbuffersize;
	short playerbuffersize;
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
