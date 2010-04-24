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
	//SpawnInfo *GetPlayerSpawnInfo(const unsigned char index, const unsigned char classindex);
	void DisconnectPlayer(const short index);
	bool MovePlayer(const short index, const float position[3], const float angle, const float speed);
	bool DuckPlayer(const short index, const bool shouldduck);
	bool PlayerEnterInVehicle(const short index, const unsigned char vehicleindex, const int seat);
	bool PlayerExitFromVehicle(const short index);
	bool FirePlayer(const short index, const int weapon, const short target, const int health, const int armour);
	bool ChangePlayerWeapon(const short index, const int weapon);
	bool SetPlayerHealth(const short index, const int health);
	bool SetPlayerArmour(const short index, const int armour);
	bool SetPlayerModel(const short index, const int model);
	bool SetPlayerComponents(const short index, const int compD[11], const int compT[11]);
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
		int health, armour;
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
	struct SpawnInfo
	{
		int model;
		float position[3], angle;
		int room;
		int CompT[11], CompD[11];
		int health, armour;
	};
	bool RegisterNewPlayer(const short index, const char *name);
	short GetPlayerFreeSlot(void);
	bool ResizePlayerBuffer(Player **&buffer, const short size);
	bool GetClassFreeSlot(unsigned char &index);
	bool ResizeClassBuffer(PlayerClass **&buffer, const unsigned char size);
	friend class NetworkManager;
};
