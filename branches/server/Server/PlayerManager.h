#pragma once

struct SpawnInfo
{
	int model;
	float position[3], angle;
	int room;
	int CompT[11], CompD[11];
	int health, armour;
};

class PlayerManager
{
public:
	PlayerManager(void);
	~PlayerManager(void);
	unsigned char GetMaxPlayers(void);
	bool IsServerFull(void);
	bool RegisterNewPlayer(const unsigned char index, char *name);
	bool IsPlayerConnected(const unsigned char index);
	char *GetPlayerName(const unsigned char index);
	bool GetPlayerColor(const unsigned char index, unsigned char (&color)[4]);
	SpawnInfo *GetPlayerSpawnInfo(const unsigned char index, const unsigned char classindex);
	void DisconnectPlayer(const unsigned char index);
	bool MovePlayer(const unsigned char index, const float position[3], const float angle, const float speed);
	bool DuckPlayer(const unsigned char index, const bool shouldduck);
	bool PlayerEnterInVehicle(const unsigned char index, const unsigned char vehicleindex, const int seat);
	bool PlayerExitFromVehicle(const unsigned char index);
	bool FirePlayer(const unsigned char index, const int weapon, const unsigned char target, const int health, const int armour);
	bool ChangePlayerWeapon(const unsigned char index, const int weapon);
	bool SetPlayerHealth(const unsigned char index, const int health);
	bool SetPlayerArmour(const unsigned char index, const int armour);
	bool SetPlayerModel(const unsigned char index, const int model);
	bool SetPlayerComponents(const unsigned char index, const int compD[11], const int compT[11]);
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
	unsigned char maxplayerbuffersize;
	unsigned char playerbuffersize;
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
	bool GetPlayerFreeSlot(unsigned char &index);
	bool ResizePlayerBuffer(Player **&buffer, const unsigned char size);
	bool GetClassFreeSlot(unsigned char &index);
	bool ResizeClassBuffer(PlayerClass **&buffer, const unsigned char size);
};
