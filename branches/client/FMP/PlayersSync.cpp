
#include "log.h"
#include "Hook\types.h"
#include "Hook\classes.h"
//#include "Hook\hook.h"
#include "Hook\scripting.h"
#include "functions.h"
#include "structs.h"
#include "main.h"
// RakNet
#include "net\RakNetworkFactory.h"
#include "net\RakPeerInterface.h"
#include "net\BitStream.h"

using namespace Natives;

extern RakPeerInterface *net;
extern int mp_state;
extern bool myEnter;


void FMPHook::PlayerConnect(char *name, int index, int model, float x, float y, float z)
{
	Debug("PlayerConnect: %s", "Start");
	Log("ConnectInfo: %s %d %d %f %f %f", name, index, model, x, y, z);
	Log("STRCMP('%s', '%s') = 0 ?", name, Conf.Name);
	if(strcmp(name, Conf.Name) == 0) // My connect
	{
		MyID = index;
		RequestModel((eModel)model);
		Debug("PlayerConnect: %s", "RequestModel");
		while(!HasModelLoaded((eModel)model)) wait(1);
		Debug("PlayerConnect: %s", "ModelLoaded");
		ChangePlayerModel(ConvertIntToPlayerIndex(GetPlayerId()), (eModel)model);
		Debug("PlayerConnect: %s", "ChangeModel");
		GetPlayerChar(ConvertIntToPlayerIndex(GetPlayerId()), &gPlayer[index].PedID);
		SetCharDefaultComponentVariation(gPlayer[index].PedID);
		SetCharCoordinates(gPlayer[index].PedID, x, y, z);
		Debug("PlayerConnect: %s", "SetCoords");
		mp_state++;
	}
	else // Other player connect
	{
		RequestModel((eModel)model);
		Debug("PlayerConnect: %s", "RequestModel");
		while(!HasModelLoaded((eModel)model)) wait(1);
		Debug("PlayerConnect: %s", "ModelLoaded");
		CreateChar(1, (eModel)model, x, y, z, &gPlayer[index].PedID, 1);
		while(DoesCharExist(gPlayer[index].PedID)) wait(1);
		SetCharDefaultComponentVariation(gPlayer[index].PedID);
		Log("MovePlayer %d(%d) = %d", index,DoesCharExist(gPlayer[index].PedID),gPlayer[index].PedID);
		Debug("PlayerConnect: %s", "CreateChar");
		GivePedFakeNetworkName(gPlayer[index].PedID, name, gPlayer[index].color[1],gPlayer[index].color[2],gPlayer[index].color[3],gPlayer[index].color[0]);
		Log("Player NAME: %s 0x%x%x%x alpha:%x",name, gPlayer[index].color[1],gPlayer[index].color[2],gPlayer[index].color[3],gPlayer[index].color[0]);
	}
	AddArmourToChar(gPlayer[index].PedID, gPlayer[index].armour);


	if(gPlayer[index].car_id != -1)
	{
		if(gPlayer[index].seat_id == -1) TaskEnterCarAsDriver(gPlayer[index].PedID, gCar[gPlayer[index].car_id].CarID, 0);
		else TaskEnterCarAsPassenger(gPlayer[index].PedID, gCar[gPlayer[index].car_id].CarID, 0, gPlayer[index].seat_id);
	}

	for(int i = 0; i < 8; i++)
	{
		if(gPlayer[index].gWeapons[i] != 0 && gPlayer[index].gAmmo[i] > 0)
			GiveWeaponToChar(gPlayer[index].PedID, (eWeapon)gPlayer[index].gWeapons[i], gPlayer[index].gAmmo[i], 0);
	}

	strcpy(gPlayer[index].name, name);
	gPlayer[index].last_active = GetTickCount();
	gPlayer[index].connected = 1;
	Debug("PlayerConnect: %s", "End");
}

void FMPHook::ReCreatePlayer(int player)
{
	RequestModel((eModel)gPlayer[player].model);
	Log("REPlayerConnect: %s", "RequestModel");
	while(!HasModelLoaded((eModel)gPlayer[player].model)) wait(1);
	Log("REPlayerConnect: %s", "ModelLoaded");
	CreateChar(1, (eModel)gPlayer[player].model, gPlayer[player].x, gPlayer[player].y, gPlayer[player].z, &gPlayer[player].PedID, 1);
	wait(1);
	Log("REMovePlayer %d(%d) = %d", player,DoesCharExist(gPlayer[player].PedID),gPlayer[player].PedID);
	Log("REPlayerConnect: %s", "CreateChar");
}

void FMPHook::Jump(int player)
{
	if(gPlayer[player].connected == 0) return;
	Log("Jump %d(%d) = %d", player,DoesCharExist(gPlayer[player].PedID),gPlayer[player].PedID);
	if(DoesCharExist(gPlayer[player].PedID) == 0) 
	{
		Log("..:::..:.SUKO.:..:::.. PLAYER IS GHOST");
		ReCreatePlayer(player);
	}
	Log("Jump Player Char Exist");
	TaskJump(gPlayer[player].PedID, 1);
	gPlayer[player].last_active = GetTickCount();
}

void FMPHook::Duck(int player, bool duck)
{
	if(gPlayer[player].connected == 0) return;
	Log("Duck %d(%d) = %d", player,DoesCharExist(gPlayer[player].PedID),gPlayer[player].PedID);
	if(DoesCharExist(gPlayer[player].PedID) == 0) 
	{
		Log("..:::..:.SUKO.:..:::.. PLAYER IS GHOST");
		ReCreatePlayer(player);
	}
	Log("Duck Player Char Exist");
	TaskToggleDuck(gPlayer[player].PedID, duck);
	gPlayer[player].last_active = GetTickCount();
	gPlayer[player].duck = duck;
}

void FMPHook::PlayerMove(int player, float x, float y, float z, float speed)
{
	Log("PlayerMOVE HOOK");
	if(gPlayer[player].connected == 0) return;
	Log("MovePlayer %d(%d) = %d", player,DoesCharExist(gPlayer[player].PedID),gPlayer[player].PedID);
	if(DoesCharExist(gPlayer[player].PedID) == 0) 
	{
		Log("..:::..:.SUKO.:..:::.. PLAYER IS GHOST");
		ReCreatePlayer(player);
	}
	Log("Move Player Char Exist");
	float lx,ly,lz;
	GetCharCoordinates(gPlayer[player].PedID, &lx, &ly, &lz);
	float d = GetDist(lx, ly, lz, x, y, z);
	if(gPlayer[player].car_id == -1) // Если пешком
	{
		Log("MovePlayer (HOOK) %s", "Start");
		int ms = 4;
		Log("Before Move");
		if(speed < 3)
		{
			ms = 2;
			if(d > 10)
			{
				SetCharCoordinates(gPlayer[player].PedID, x, y, z);
				SetCharHeading(gPlayer[player].PedID, gPlayer[player].angle);
				Log("PORTAL or TIMESHIFT");
			}
			Log("MovePlayer %d(%d) = %d", player,DoesCharExist(gPlayer[player].PedID),gPlayer[player].PedID);
		}
		else if(speed < 5)
		{
			ms = 3;
			if(d > 20)
			{
				SetCharCoordinates(gPlayer[player].PedID, x, y, z);
				SetCharHeading(gPlayer[player].PedID, gPlayer[player].angle);
				Log("PORTAL or TIMESHIFT");
			}
			Log("MovePlayer %d(%d) = %d", player,DoesCharExist(gPlayer[player].PedID),gPlayer[player].PedID);
		}
		else
		{
			ms = 4;
			if(d > 30)
			{
				SetCharCoordinates(gPlayer[player].PedID, x, y, z);
				SetCharHeading(gPlayer[player].PedID, gPlayer[player].angle);
				Log("PORTAL or TIMESHIFT");
			}
			Log("MovePlayer %d(%d) = %d", player,DoesCharExist(gPlayer[player].PedID),gPlayer[player].PedID);
		}
		Log("MOVE %f=(%f) %d", speed, d, ms);
		TaskGoStraightToCoord(gPlayer[player].PedID, x, y, z, ms, 45000);
		wait(1);
		gPlayer[player].x = x;
		gPlayer[player].y = y;
		gPlayer[player].z = z;
		Log("MovePlayer %d(%d) = %d", player,DoesCharExist(gPlayer[player].PedID),gPlayer[player].PedID);
	}
	else // Если на машине
	{
		int vect = 1;
		if(speed < 0) { vect = 2; speed = speed*-1; }
		if(speed * 3 < d)
		{
			SetCharCoordinates(gPlayer[player].PedID, x, y, z);
			SetCarHeading(gCar[gPlayer[player].car_id].CarID, gCar[gPlayer[player].car_id].angle);
		}

		TaskCarDriveToCoord(gPlayer[player].PedID, gCar[gPlayer[player].car_id].CarID, x, y, z, speed, vect, 2, 3, 2, 45000000);
	}
	gPlayer[player].last_active = GetTickCount();
}

void FMPHook::PlayerDisconnect(int id)
{
	Log("DELETE CHAR %d", id);
	DeleteChar(&gPlayer[id].PedID);
	gPlayer[id].connected = 0;
}

void FMPHook::CancelEnterInVehicle(int id)
{
	Log("CancelEnterInVehicle %d", id);
	ClearCharTasks(gPlayer[id].PedID);
	gPlayer[id].car_id = -1;
}

void FMPHook::ExitFromVehicle(int id)
{
	Log("ExitFromVehicle %d", id);
	TaskLeaveAnyCar(gPlayer[id].PedID);
	gPlayer[id].car_id = -1;
}

void FMPHook::EnterInVehicle(int id, int car, int seat)
{
	Log("EnterInVehicle %d, %d, %d", id, car, seat);
	if(seat == -1) TaskEnterCarAsDriver(gPlayer[id].PedID, gCar[car].CarID, -1);
	else TaskEnterCarAsPassenger(gPlayer[id].PedID, gCar[car].CarID, -1, seat);
	gPlayer[id].car_id = car;
	gPlayer[id].seat_id = seat;
}

void FMPHook::PlayerFireAim(int playerid, int gun, int time, float x, float y, float z, bool fire)
{
	if(fire)
	{
		SetCurrentCharWeapon(gPlayer[playerid].PedID, (eWeapon)gun, 1);
		TaskShootAtCoord(gPlayer[playerid].PedID, x, y, z, time, 5);
	}
	else
	{
		SetCurrentCharWeapon(gPlayer[playerid].PedID, (eWeapon)gun, 1);
		TaskAimGunAtCoord(gPlayer[playerid].PedID, x, y, z, time);
	}
}

void FMPHook::PlayerSwapGun(int player, int gun)
{
	TaskSwapWeapon(gPlayer[player].PedID, (eWeapon)gun);
}

void FMPHook::PlayerSyncSkin(int player, int skin)
{
	if(player == MyID)
	{
		RequestModel((eModel)skin);
		while(!HasModelLoaded((eModel)skin)) wait(0);
		ChangePlayerModel(GetPlayerId(), (eModel)skin);
		
	}
	else
	{
		RequestModel((eModel)skin);
		while(!HasModelLoaded((eModel)skin)) wait(0);
		ChangePlayerModel(GetPlayerId(), (eModel)skin);
	}
}

void FMPHook::PlayerSyncSkinVariation(int player, int sm[11], int st[11])
{
	for(int i = 0; i < 11; i++)
		SetCharComponentVariation(gPlayer[player].PedID, (ePedComponent)i, sm[i], st[i]);  
}

void FMPHook::PlayerSpawn(int player, SpawnInfo spawn)
{
	int model;
	GetCharModel(gPlayer[player].PedID, (eModel*)&model);
	if(spawn.model != model) PlayerSyncSkin(player, spawn.model);

	PlayerSyncSkinVariation(player, spawn.CompD, spawn.CompT);

	SetCharCoordinates(gPlayer[player].PedID, spawn.x, spawn.y, spawn.z);
	SetCharHeading(gPlayer[player].PedID, spawn.r);
	SetCharHealth(gPlayer[player].PedID, spawn.health);
	AddArmourToChar(gPlayer[player].PedID, -1000);
	AddArmourToChar(gPlayer[player].PedID, spawn.armour);
	SetRoomForCharByKey(gPlayer[player].PedID, (eInteriorRoomKey)spawn.room);
}