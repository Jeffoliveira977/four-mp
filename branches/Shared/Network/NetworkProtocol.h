
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
	int armour;
	int room;
	int weapons[8];
	int ammo[8];
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