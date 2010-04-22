#include <stdlib.h>

#include "PlayerManager.h"
#include "logging.h"
#include "NetworkManager.h"
#include "VirtualMachineManager.h"
#include "VehicleManager.h"

extern NetworkManager nm;
extern VirtualMachineManager vmm;
extern VehicleManager vm;

PlayerManager::PlayerManager(void)
{
	maxplayerbuffersize = 32;
	maxclassbuffersize = 64;
	playerbuffersize = 0;
	classbuffersize = 0;
}

PlayerManager::~PlayerManager(void)
{
}

unsigned char PlayerManager::GetMaxPlayers(void)
{
	return maxplayerbuffersize;
}

bool PlayerManager::IsServerFull(void)
{
	if (playerbuffersize < maxplayerbuffersize)
	{
		return false;
	}
	for (unsigned char i = 0; i < playerbuffersize; i++)
	{
		if (playerbuffer[i] == NULL)
		{
			return false;
		}
	}
	return true;
}

bool PlayerManager::RegisterNewPlayer(const unsigned char index, char *name)
{
	if (playerbuffer[index] != NULL)
	{
		nm.SendConnectionError(index, ConnectionErrorAllocationError);
		nm.RemoveClient(index);
		return false;
	}
	if (index >= playerbuffersize)
	{
		if (playerbuffersize == maxplayerbuffersize)
		{
			nm.SendConnectionError(index, ConnectionErrorAllocationError);
			nm.RemoveClient(index);
			return false;
		}
		if (!this->ResizePlayerBuffer(playerbuffer, index + 1))
		{
			nm.SendConnectionError(index, ConnectionErrorAllocationError);
			nm.RemoveClient(index);
			return false;
		}
		playerbuffer[index] = NULL;
		playerbuffersize = index + 1;
	}
	if (vmm.OnPlayerConnect(index, name) == 0)
	{
		nm.SendConnectionError(index, ConnectionErrorScriptLock);
		nm.RemoveClient(index);
		return false;
	}
	playerbuffer[index] = new Player;
	strcpy(playerbuffer[index]->name, name);
	playerbuffer[index]->model = 0x98E29920;
	playerbuffer[index]->position[0] = 0;
	playerbuffer[index]->position[1] = 0;
	playerbuffer[index]->position[2] = 0;
	playerbuffer[index]->angle = 0;
	playerbuffer[index]->last_active = 0;
	playerbuffer[index]->sync_state = 0;
	for(unsigned char i = 0; i < 8; i++)
	{
		playerbuffer[index]->weapons[i] = 0;
		playerbuffer[index]->ammo[i] = 0;
	}
	playerbuffer[index]->animation[0] = 0;
	playerbuffer[index]->vehicleindex = -1;
	playerbuffer[index]->seat_id = 99;
	playerbuffer[index]->score = 0;
	playerbuffer[index]->health = 200;
	playerbuffer[index]->armour = 0;
	playerbuffer[index]->wanted_level = 0;
	playerbuffer[index]->edSprint = 1;
	playerbuffer[index]->edLockon = 1;
	playerbuffer[index]->edDoDriveBy = 1;
	playerbuffer[index]->edUseCover = 1;
	playerbuffer[index]->edConrol = 1;
	playerbuffer[index]->edFreeze = 0;
	playerbuffer[index]->isducking = 0;
	playerbuffer[index]->room = 0;
	playerbuffer[index]->car_enter = 0;
	playerbuffer[index]->color[0] = 0xFF;
	playerbuffer[index]->color[1] = 0xFF;
	playerbuffer[index]->color[2] = 0x00;
	playerbuffer[index]->color[3] = 0x00;
	playerbuffer[index]->currentweapon = 0;
	nm.SendNewPlayerInfoToAll(index, name, playerbuffer[index]->model, playerbuffer[index]->position, playerbuffer[index]->angle, playerbuffer[index]->vehicleindex, playerbuffer[index]->seat_id, playerbuffer[index]->score, playerbuffer[index]->health, playerbuffer[index]->armour, playerbuffer[index]->room, playerbuffer[index]->weapons, playerbuffer[index]->ammo, playerbuffer[index]->color);
	for (unsigned char i = 0; i < playerbuffersize; i++)
	{
		if (playerbuffer[i] != NULL)
		{
			nm.SendNewPlayerInfo(index, i, playerbuffer[i]->name, playerbuffer[i]->model, playerbuffer[i]->position, playerbuffer[i]->angle, playerbuffer[i]->vehicleindex, playerbuffer[i]->seat_id, playerbuffer[i]->score, playerbuffer[i]->health, playerbuffer[i]->armour, playerbuffer[i]->room, playerbuffer[i]->weapons, playerbuffer[i]->ammo, playerbuffer[i]->color);
		}
	}
	unsigned char maxvehicles = vm.GetVehicleBufferSize();
	for(unsigned char i = 0; i < maxvehicles; i++)
	{
		if(vm.IsVehicleExists(i))
		{
			vm.SendVehicleInfo(index, i);
		}
	}
	nm.SendClassInfo(index);
	return true;
}

bool PlayerManager::IsPlayerConnected(const unsigned char index)
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	return true;
}

char *PlayerManager::GetPlayerName(const unsigned char index)
{
	if (index >= playerbuffersize)
	{
		return NULL;
	}
	if (playerbuffer[index] == NULL)
	{
		return NULL;
	}
	return playerbuffer[index]->name;
}

bool PlayerManager::GetPlayerColor(const unsigned char index, unsigned char (&color)[4])
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	color[0] = playerbuffer[index]->color[0];
	color[1] = playerbuffer[index]->color[1];
	color[2] = playerbuffer[index]->color[2];
	color[3] = playerbuffer[index]->color[3];
	return true;
}

SpawnInfo *PlayerManager::GetPlayerSpawnInfo(const unsigned char index, const unsigned char classindex)
{
	if (index >= playerbuffersize)
	{
		return NULL;
	}
	if (playerbuffer[index] == NULL)
	{
		return NULL;
	}
	if (classindex >= classbuffersize)
	{
		return NULL;
	}
	if (classbuffer[classindex] == NULL)
	{
		return NULL;
	}
	SpawnInfo *spawninfo = new SpawnInfo;
	spawninfo->armour = 0;
	spawninfo->health = 200;
	spawninfo->model = classbuffer[classindex]->model;
	spawninfo->angle = classbuffer[classindex]->angle;
	spawninfo->room = 0;
	spawninfo->position[0] = classbuffer[classindex]->position[0];
	spawninfo->position[1] = classbuffer[classindex]->position[1];
	spawninfo->position[2] = classbuffer[classindex]->position[2];
	for (unsigned char i = 0; i < 11; i++)
	{
		spawninfo->CompD[i] = playerbuffer[index]->compD[i];
		spawninfo->CompT[i] = playerbuffer[index]->compT[i];
	}
	return spawninfo;
}

void PlayerManager::DisconnectPlayer(const unsigned char index)
{
	if (index >= playerbuffersize)
	{
		return;
	}
	if (playerbuffer[index] == NULL)
	{
		return;
	}
	vmm.OnPlayerDisconnect(index);
	PrintToServer("Player %s disconnected", playerbuffer[index]->name);
	free(playerbuffer[index]);
	playerbuffer[index] = NULL;
	nm.DisconnectClient(index);
}

bool PlayerManager::MovePlayer(const unsigned char index, const float position[], const float angle, const float speed)
{
	if (index >= playerbuffersize)
	{
		return false;;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	if (playerbuffer[index]->vehicleindex != -1)
	{
		vm.SetVehiclePosition(playerbuffer[index]->vehicleindex, position);
		vm.SetVehicleAngle(playerbuffer[index]->vehicleindex, angle);
	}

	playerbuffer[index]->position[0] = position[0];
	playerbuffer[index]->position[1] = position[1];
	playerbuffer[index]->position[2] = position[2];
	playerbuffer[index]->angle = angle;

	if (playerbuffer[index]->car_enter == 1)
	{
		return false;
	}
	return true;
}

bool PlayerManager::DuckPlayer(const unsigned char index, const bool shouldduck)
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	playerbuffer[index]->isducking = shouldduck;
	return true;
}

bool PlayerManager::PlayerEnterInVehicle(const unsigned char index, const unsigned char vehicleindex, const int seat)
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	if (seat == 100)
	{
		playerbuffer[index]->car_enter = false;
		return false;
	}
	playerbuffer[index]->car_enter = true;
	playerbuffer[index]->seat_id = seat;
	return true;
}

bool PlayerManager::PlayerExitFromVehicle(const unsigned char index)
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	playerbuffer[index]->vehicleindex = -1;
	playerbuffer[index]->car_enter = false;
	return true;
}

bool PlayerManager::FirePlayer(const unsigned char index, const int weapon, const unsigned char target, const int health, const int armour)
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	if (target >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[target] == NULL)
	{
		return false;
	}
	playerbuffer[index]->currentweapon = weapon;
	if(target != -1)
	{
		playerbuffer[target]->health = health;
		playerbuffer[target]->armour = armour;
	}
	return true;
}

bool PlayerManager::ChangePlayerWeapon(const unsigned char index, const int weapon)
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	playerbuffer[index]->currentweapon = weapon;
	return true;
}

bool PlayerManager::SetPlayerHealth(const unsigned char index, const int health)
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	playerbuffer[index]->health = health;
	return true;
}

bool PlayerManager::SetPlayerArmour(const unsigned char index, const int armour)
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	playerbuffer[index]->armour = armour;
	return true;
}

bool PlayerManager::SetPlayerModel(const unsigned char index, const int model)
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	playerbuffer[index]->model = model;
	return true;
}

bool PlayerManager::SetPlayerComponents(const unsigned char index, const int compD[11], const int compT[11])
{
	if (index >= playerbuffersize)
	{
		return false;
	}
	if (playerbuffer[index] == NULL)
	{
		return false;
	}
	for (unsigned char i = 0; i < 11; i++)
	{
		playerbuffer[index]->compD[i] = compD[i];
		playerbuffer[index]->compT[i] = compT[i];
	}
	return true;
}

unsigned char PlayerManager::GetNumberOfPlayerClasses(void)
{
	unsigned char numclasses = 0;
	for (unsigned char i = 0; i < classbuffersize; i++)
	{
		if (classbuffer[i] != NULL)
		{
			numclasses++;
		}
	}
	return numclasses;
}

bool PlayerManager::AddPlayerClass(const int model, const float position[3], const float angle, const int weapons[8], const int ammo[8], unsigned char &index)
{
	if (!this->GetClassFreeSlot(index))
	{
		PrintToServer("Unable to add player class. No free slots.");
		return false;
	}
	if (index >= classbuffersize)
	{
		if (classbuffersize == maxclassbuffersize)
		{
			return false;
		}
		if (!this->ResizeClassBuffer(classbuffer, index + 1))
		{
			return false;
		}
		classbuffer[index] = NULL;
		classbuffersize = index + 1;
	}
	classbuffer[index] = new PlayerClass;
	classbuffer[index]->model = model;
	classbuffer[index]->position[0] = position[0];
	classbuffer[index]->position[1] = position[1];
	classbuffer[index]->position[2] = position[2];
	classbuffer[index]->angle = angle;
	classbuffer[index]->weapons[0] = weapons[0];
	classbuffer[index]->weapons[1] = weapons[1];
	classbuffer[index]->weapons[2] = weapons[2];
	classbuffer[index]->weapons[3] = weapons[3];
	classbuffer[index]->weapons[4] = weapons[4];
	classbuffer[index]->weapons[5] = weapons[5];
	classbuffer[index]->weapons[6] = weapons[6];
	classbuffer[index]->weapons[7] = weapons[7];
	classbuffer[index]->ammo[0] = ammo[0];
	classbuffer[index]->ammo[1] = ammo[1];
	classbuffer[index]->ammo[2] = ammo[2];
	classbuffer[index]->ammo[3] = ammo[3];
	classbuffer[index]->ammo[4] = ammo[4];
	classbuffer[index]->ammo[5] = ammo[5];
	classbuffer[index]->ammo[6] = ammo[6];
	classbuffer[index]->ammo[7] = ammo[7];
	//TODO: sync it? Does classes even sync?
}

bool PlayerManager::GetPlayerClassData(const unsigned char index, int &model, float (&position)[3], float &angle, int (&weapons)[8], int (&ammo)[8])
{
	if (index >= classbuffersize)
	{
		return false;
	}
	if (classbuffer[index] == NULL)
	{
		return false;
	}
	model = classbuffer[index]->model;
	position[0] = classbuffer[index]->position[0];
	position[1] = classbuffer[index]->position[1];
	position[2] = classbuffer[index]->position[2];
	angle = classbuffer[index]->angle;
	weapons[0] = classbuffer[index]->weapons[0];
	weapons[1] = classbuffer[index]->weapons[1];
	weapons[2] = classbuffer[index]->weapons[2];
	weapons[3] = classbuffer[index]->weapons[3];
	weapons[4] = classbuffer[index]->weapons[4];
	weapons[5] = classbuffer[index]->weapons[5];
	weapons[6] = classbuffer[index]->weapons[6];
	weapons[7] = classbuffer[index]->weapons[7];
	ammo[0] = classbuffer[index]->ammo[0];
	ammo[1] = classbuffer[index]->ammo[1];
	ammo[2] = classbuffer[index]->ammo[2];
	ammo[3] = classbuffer[index]->ammo[3];
	ammo[4] = classbuffer[index]->ammo[4];
	ammo[5] = classbuffer[index]->ammo[5];
	ammo[6] = classbuffer[index]->ammo[6];
	ammo[7] = classbuffer[index]->ammo[7];
	return true;
}

bool PlayerManager::GetPlayerFreeSlot(unsigned char &index)
{
	for (index = 0; index < playerbuffersize; index++)
	{
		if (playerbuffer[index] == NULL)
		{
			return true;
		}
	}
	if (playerbuffersize == maxplayerbuffersize)
	{
		return false;
	}
	return true;
}

bool PlayerManager::ResizePlayerBuffer(Player **&buffer, const unsigned char size)
{
	Player **tempbuffer = (Player **)realloc(*&buffer, size * sizeof(Player *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

bool PlayerManager::GetClassFreeSlot(unsigned char &index)
{
	for (index = 0; index < classbuffersize; index++)
	{
		if (classbuffer[index] == NULL)
		{
			return true;
		}
	}
	if (classbuffersize == maxclassbuffersize)
	{
		return false;
	}
	return true;
}

bool PlayerManager::ResizeClassBuffer(PlayerClass **&buffer, const unsigned char size)
{
	PlayerClass **tempbuffer = (PlayerClass **)realloc(*&buffer, size * sizeof(PlayerClass *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}