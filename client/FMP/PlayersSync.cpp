#include <vector>

#include "log.h"
#include "Hook\types.h"
#include "Hook\classes.h"
//#include "Hook\hook.h"
#include "Hook\scripting.h"
#include "functions.h"
#include "structs.h"
#include "main.h"
#include "../Shared/ClientCore.h"
#include "hook\classes.h"

scrThread* GetActiveThread();
void SetActiveThread(scrThread* thread);

extern ClientCore client;
extern FMPHook HOOK;

void FMPHook::PlayerConnect(char *name, short index, unsigned int model, float position[3])
{
	Log::Debug("PlayerConnect: %s", "Start");
	if(name == 0)
	{
		Log::Error("Can't connecting player (Name is bad)");
		return;
	}
	if(index < 0 || index > MAX_PLAYERS)
	{
		Log::Error("Can't connecting player (bad index)");
		return;
	}

	Log::Info("ConnectInfo: %s %d 0x%x %f %f %f", name, index, model, position[0], position[1], position[2]);
	if(client.GetIndex() == index) // My connect
	{
		Log::Debug("Our PlayerConnect");
		gPlayer[index].model = model;
		Log::Info("Local player %d", Natives::IsThisModelAPed((eModel)model));
		Natives::RequestModel((eModel)model);
		Log::Debug("PlayerConnect: %s", "RequestModel");
		while(!Natives::HasModelLoaded((eModel)model)) wait(1);
		Log::Debug("PlayerConnect: %s", "ModelLoaded");
		Natives::ChangePlayerModel(_GetPlayer(), (eModel)model);
		Log::Debug("PlayerConnect: %s", "ChangeModel");
		Natives::GetPlayerChar(_GetPlayer(), &gPlayer[index].PedID);
		Natives::SetCharCoordinates(gPlayer[index].PedID, position[0], position[1], position[2]);
		Log::Debug("PlayerConnect: %s", "SetCoords");
		client.SetGameState(GameStateInGame);
		client.SetInputState(InputStateGame);
		HOOK.InputFreeze(0);
	}
	else // Other player connect
	{
		Log::Debug("Other player connect");
		Natives::RequestModel((eModel)model);
		Log::Debug("PlayerConnect: %s", "RequestModel");
		while(!Natives::HasModelLoaded((eModel)model)) wait(1);
		Log::Debug("PlayerConnect: %s", "ModelLoaded");
		Natives::CreateChar(1, (eModel)model, position[0], position[1], position[2], &gPlayer[index].PedID, 1);
		Log::Debug("PlayerConnect: %s", "CreateChar");
		while(!Natives::DoesCharExist(gPlayer[index].PedID)) wait(1);
		Log::Info("MovePlayer %d(%d) = %d", index, Natives::DoesCharExist(gPlayer[index].PedID),gPlayer[index].PedID);
		Natives::GivePedFakeNetworkName(gPlayer[index].PedID, name, gPlayer[index].color[1],gPlayer[index].color[2],gPlayer[index].color[3],gPlayer[index].color[0]);
		Log::Info("Player NAME: %s 0x%x%x%x alpha:%x",name, gPlayer[index].color[1],gPlayer[index].color[2],gPlayer[index].color[3],gPlayer[index].color[0]);
		Natives::SetBlockingOfNonTemporaryEvents(gPlayer[index].PedID, 1);
	}

	Natives::SetCharDefaultComponentVariation( gPlayer[index].PedID );
	Natives::AddArmourToChar(gPlayer[index].PedID, gPlayer[index].armor);
	gPlayer[index].model = model;
	memcpy(gPlayer[index].position, position, sizeof(float) * 3);

	if(gPlayer[index].vehicleindex != -1)
	{
		if(gPlayer[index].seatindex == -1) Natives::TaskEnterCarAsDriver(gPlayer[index].PedID, gCar[gPlayer[index].vehicleindex].CarID, 0);
		else Natives::TaskEnterCarAsPassenger(gPlayer[index].PedID, gCar[gPlayer[index].vehicleindex].CarID, 0, gPlayer[index].seatindex);
	}

	for(int i = 0; i < 8; i++)
	{
		if(gPlayer[index].weapons[i] != 0 && gPlayer[index].ammo[i] > 0)
			Natives::GiveWeaponToChar(gPlayer[index].PedID, (eWeapon)gPlayer[index].weapons[i], gPlayer[index].ammo[i], 0);
	}

	strcpy(gPlayer[index].name, name);
	gPlayer[index].last_active = GetTickCount();
	gPlayer[index].connected = 1;
	Log::Debug("PlayerConnect: %s", "End");
}

void FMPHook::Jump(short index)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info("Jump %d(%d) = %d", index, Natives::DoesCharExist(gPlayer[index].PedID), gPlayer[index].PedID);
	Natives::TaskJump(gPlayer[index].PedID, 1);
	gPlayer[index].last_active = GetTickCount();
}

void FMPHook::Duck(short index, bool duck)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info("Duck %d(%d) = %d", index, Natives::DoesCharExist(gPlayer[index].PedID) ,gPlayer[index].PedID);
	Natives::TaskToggleDuck(gPlayer[index].PedID, duck);
	gPlayer[index].last_active = GetTickCount();
	gPlayer[index].isducking = duck;
}

void FMPHook::PlayerMove(short index, float position[3], float speed)
{
	Log::Info("PlayerMOVE HOOK");
	Log::Info("Index: %d, position: %f %f %f, speed: %f", index, position[0], position[1], position[2], speed);
	
	if(!SafeCheckPlayer(index)) return;

	float lx,ly,lz;
	Natives::GetCharCoordinates(gPlayer[index].PedID, &lx, &ly, &lz);
	Log::Info("GETCOORD");
	float d = GetDist(lx, ly, lz, position[0], position[1], position[2]);
	if(gPlayer[index].vehicleindex == -1) // Если пешком
	{
		Log::Info("MovePlayer (HOOK) %s", "Start");
		int ms = 4;
		Log::Info("Before Move");
		if(speed < 3)
		{
			ms = 2;
			if(d > 10)
			{
				Natives::SetCharCoordinates(gPlayer[index].PedID, position[0], position[1], position[2]);
				Natives::SetCharHeading(gPlayer[index].PedID, gPlayer[index].angle);
				Log::Info("PORTAL or TIMESHIFT");
			}
		}
		else if(speed < 5)
		{
			ms = 3;
			if(d > 20)
			{
				Natives::SetCharCoordinates(gPlayer[index].PedID, position[0], position[1], position[2]);
				Natives::SetCharHeading(gPlayer[index].PedID, gPlayer[index].angle);
				Log::Info("PORTAL or TIMESHIFT");
			}
		}
		else
		{
			ms = 4;
			if(d > 30)
			{
				Natives::SetCharCoordinates(gPlayer[index].PedID, position[0], position[1], position[2]);
				Natives::SetCharHeading(gPlayer[index].PedID, gPlayer[index].angle);
				Log::Info("PORTAL or TIMESHIFT");
			}
		}
		Log::Info("MOVE %f=(%f) %d", speed, d, ms);
		Natives::TaskGoStraightToCoord(gPlayer[index].PedID, position[0], position[1], position[2], ms, 45000);
		wait(1);
		memcpy(gPlayer[index].position, position, sizeof(float) * 3);
	}
	else // Если на машине
	{
		int vect = 1;
		if(speed < 0) { vect = 2; speed = speed*-1; }
		if(speed * 3 < d)
		{
			Natives::SetCharCoordinates(gPlayer[index].PedID, position[0], position[1], position[2]);
			Natives::SetCarHeading(gCar[gPlayer[index].vehicleindex].CarID, gCar[gPlayer[index].vehicleindex].angle);
		}

		Natives::TaskCarDriveToCoord(gPlayer[index].PedID, gCar[gPlayer[index].vehicleindex].CarID, position[0], position[1], position[2], speed, vect, 2, 3, 2, 45000000);
	}
	gPlayer[index].last_active = GetTickCount();
}

void FMPHook::PlayerDisconnect(short index)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info("DELETE CHAR %d", index);
	Natives::DeleteChar(&gPlayer[index].PedID);
	gPlayer[index].connected = 0;
}

void FMPHook::CancelEnterInVehicle(short index)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info("CancelEnterInVehicle %d", index);
	Natives::ClearCharTasks(gPlayer[index].PedID);
	gPlayer[index].vehicleindex = -1;
}

void FMPHook::ExitFromVehicle(short index)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info("ExitFromVehicle %d", index);
	Natives::TaskLeaveAnyCar(gPlayer[index].PedID);
	gPlayer[index].vehicleindex = -1;
}

void FMPHook::EnterInVehicle(short index, int car, int seat)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info("EnterInVehicle %d, %d, %d", index, car, seat);
	if(seat == 0) Natives::TaskEnterCarAsDriver(gPlayer[index].PedID, gCar[car].CarID, -1);
	else if(seat > 0) Natives::TaskEnterCarAsPassenger(gPlayer[index].PedID, gCar[car].CarID, -1, seat-1);
	gPlayer[index].vehicleindex = car;
	gPlayer[index].seatindex = seat;
}

void FMPHook::PlayerFireAim(short index, int gun, int time, float x, float y, float z, bool fire)
{
	if(!SafeCheckPlayer(index)) return;

	if(fire)
	{
		Natives::SetCurrentCharWeapon(gPlayer[index].PedID, (eWeapon)gun, 1);
		Natives::TaskShootAtCoord(gPlayer[index].PedID, x, y, z, time, 5);
	}
	else
	{
		Natives::SetCurrentCharWeapon(gPlayer[index].PedID, (eWeapon)gun, 1);
		Natives::TaskAimGunAtCoord(gPlayer[index].PedID, x, y, z, time);
	}
}

void FMPHook::PlayerSwapGun(short index, int gun)
{
	if(!SafeCheckPlayer(index)) return;

	Natives::TaskSwapWeapon(gPlayer[index].PedID, (eWeapon)gun);
}

void FMPHook::PlayerSyncSkin(short index, int skin)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info("Player Sync Skin START");
	//Totally bug here.
	if(index == client.GetIndex())
	{
		Natives::RequestModel((eModel)skin);
		while(!Natives::HasModelLoaded((eModel)skin)) wait(0);
		Natives::ChangePlayerModel(_GetPlayer(), (eModel)skin);
	}
	else
	{
		/*Natives::RequestModel((eModel)skin);
		while(!Natives::HasModelLoaded((eModel)skin)) wait(0);
		Natives::ChangePlayerModel(_GetPlayer(), (eModel)skin);*/
		Log::Warning("Other player can't change model");
	}
	Log::Info("Player Sync Skin END");
}

void FMPHook::PlayerSyncSkinVariation(short index, int sm[11], int st[11])
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info("Player Sync Skin Variation START");
	for(int i = 0; i < 11; i++)
		Natives::SetCharComponentVariation(gPlayer[index].PedID, (ePedComponent)i, sm[i], st[i]); 
	Log::Info("Player Sync Skin Variantion END");
}

void FMPHook::xPlayerSpawn(NetworkPlayerSpawnData data)
{
	if(!SafeCheckPlayer(data.client)) return;
	Log::Info("Player Spawn START");
	unsigned int model;
	Natives::GetCharModel(gPlayer[data.client].PedID, (eModel*)&model);
	Log::Info("Get Model");
	if(data.model != model) PlayerSyncSkin(data.client, data.model);
	Log::Info("IF SPAWN MODEL");

	PlayerSyncSkinVariation(data.client, data.compD, data.compT);

	Log::Info("Set UP");
	Natives::SetCharCoordinates(gPlayer[data.client].PedID, data.position[0], data.position[1], data.position[2]);
	Natives::SetCharHeading(gPlayer[data.client].PedID, data.angle);
	Natives::SetCharHealth(gPlayer[data.client].PedID, data.health);
	Natives::AddArmourToChar(gPlayer[data.client].PedID, -1000);
	Natives::AddArmourToChar(gPlayer[data.client].PedID, data.armor);
	Natives::SetRoomForCharByKey(gPlayer[data.client].PedID, (eInteriorRoomKey)data.room);
	Log::Info("Player SPAWN END");
}