#include <vector>

#include "log.h"
#include "Hook/types.h"
#include "Hook/classes.h"
//#include "Hook\hook.h"
#include "Hook/scripting.h"
#include "functions.h"
#include "../Shared/ClientCore.h"
#include "PlayerManager.h"
#include "VehicleManager.h"
#include "../Shared/NetworkManager.h"

scrThread* GetActiveThread();
void SetActiveThread(scrThread* thread);

extern ClientCore client;
extern FMPHook HOOK;
extern NetworkManager nm;

extern FPlayer gPlayer[MAX_PLAYERS];
extern FVehicle gVehicle[MAX_VEHICLES];

void FMPHook::PlayerConnect(wchar_t *name, short index, unsigned int model, float position[3], bool start)
{
	Log::Debug(L"PlayerConnect: Start");
	if(name == 0)
	{
		Log::Error(L"Can't connecting player (Name is bad)");
		return;
	}
	if(index < 0 || index > MAX_PLAYERS)
	{
		Log::Error(L"Can't connecting player (bad index)");
		return;
	}

	Log::Info(L"ConnectInfo: %s %d 0x%x %f %f %f", name, index, model, position[0], position[1], position[2]);
	if(client.GetIndex() == index) // My connect
	{
		Log::Debug(L"Our PlayerConnect");
		if(start)
		{
			//Natives::SetCharHealth(_GetPlayerPed(), 0);
			nm.SendPlayerSpawnRequest();
			//gPlayer[index].want_spawn = 1;
			gPlayer[index].sync_state = 1;

			wcscpy(gPlayer[index].name, name);
			gPlayer[index].last_active = GetTickCount();
			gPlayer[index].connected = 1;

			client.SetGameState(GameStateInGame);
			client.SetInputState(InputStateGame);
			
			InputFreeze(0);
			//SetFreeCam(0);

			Log::Debug(L"PlayerConnect: fastEnd");
			return;
		}
		else
		{
			Log::Info(L"Local player %d", Natives::IsThisModelAPed((eModel)model));
			Natives::RequestModel((eModel)model);
			Log::Debug(L"PlayerConnect: RequestModel");
			while(!Natives::HasModelLoaded((eModel)model)) wait(1);
			Log::Debug(L"PlayerConnect: ModelLoaded");
			Natives::ChangePlayerModel(_GetPlayer(), (eModel)model);
			Log::Debug(L"PlayerConnect: ChangeModel");
			Natives::GetPlayerChar(_GetPlayer(), &gPlayer[index].PedID);
			Natives::SetCharCoordinates(gPlayer[index].PedID, position[0], position[1], position[2]);
			Log::Debug(L"PlayerConnect: SetCoords");
		}

		client.SetGameState(GameStateInGame);
		client.SetInputState(InputStateGame);
		
		InputFreeze(0);
		SetFreeCam(0);
	}
	else // Other player connect
	{
		Log::Debug(L"Other player connect");
		Natives::RequestModel((eModel)model);
		Log::Debug(L"PlayerConnect: RequestModel");
		while(!Natives::HasModelLoaded((eModel)model)) wait(1);
		Log::Debug(L"PlayerConnect: ModelLoaded");
		Natives::CreateChar(1, (eModel)model, position[0], position[1], position[2], &gPlayer[index].PedID, 1);
		Log::Debug(L"PlayerConnect: CreateChar");
		while(!Natives::DoesCharExist(gPlayer[index].PedID)) wait(1);
		Log::Info(L"MovePlayer %d(%d) = %d", index, Natives::DoesCharExist(gPlayer[index].PedID),gPlayer[index].PedID);
		size_t length = (sizeof(wchar_t) / sizeof(char)) * wcslen(name) + 1;
		char *tempname = (char *)calloc(length, sizeof(char));
		wcstombs(tempname, name, length);
		Natives::GivePedFakeNetworkName(gPlayer[index].PedID, tempname, gPlayer[index].color[1],gPlayer[index].color[2],gPlayer[index].color[3],gPlayer[index].color[0]);
		free(tempname);
		Natives::AddBlipForChar(gPlayer[index].PedID, &gPlayer[index].radarblip);
		Log::Info(L"Player NAME: %s 0x%x%x%x alpha:%x",name, gPlayer[index].color[1],gPlayer[index].color[2],gPlayer[index].color[3],gPlayer[index].color[0]);
		Natives::SetBlockingOfNonTemporaryEvents(gPlayer[index].PedID, 1);
		Natives::SetCharInvincible(gPlayer[index].PedID, 1);
	}

	Natives::SetCharDefaultComponentVariation( gPlayer[index].PedID );
	Natives::AddArmourToChar(gPlayer[index].PedID, gPlayer[index].armor);
	gPlayer[index].model = model;
	memcpy(gPlayer[index].position, position, sizeof(float) * 3);

	if(gPlayer[index].vehicleindex != -1)
	{
		if(gPlayer[index].seatindex == -1) Natives::WarpCharIntoCar(gPlayer[index].PedID, gVehicle[gPlayer[index].vehicleindex].CarID);
		else Natives::WarpCharIntoCarAsPassenger(gPlayer[index].PedID, gVehicle[gPlayer[index].vehicleindex].CarID, gPlayer[index].seatindex);
	}

	for(int i = 0; i < 8; i++)
	{
		if(gPlayer[index].weapons[i] != 0 && gPlayer[index].ammo[i] > 0)
			Natives::GiveWeaponToChar(gPlayer[index].PedID, (eWeapon)gPlayer[index].weapons[i], gPlayer[index].ammo[i], 0);
	}

	wcscpy(gPlayer[index].name, name);
	gPlayer[index].last_active = GetTickCount();
	gPlayer[index].connected = 1;
	Log::Debug(L"PlayerConnect: End");
}

void FMPHook::Jump(short index)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info(L"Jump %d(%d) = %d", index, Natives::DoesCharExist(gPlayer[index].PedID), gPlayer[index].PedID);
	Natives::TaskJump(gPlayer[index].PedID, 1);
	gPlayer[index].last_active = GetTickCount();
}

void FMPHook::Duck(short index, bool duck)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info(L"Duck %d(%d) = %d", index, Natives::DoesCharExist(gPlayer[index].PedID) ,gPlayer[index].PedID);
	Natives::TaskToggleDuck(gPlayer[index].PedID, duck);
	gPlayer[index].last_active = GetTickCount();
	gPlayer[index].isducking = duck;
}

void FMPHook::PlayerMove(short index, float position[3], float speed)
{
	Log::Info(L"PlayerMOVE HOOK");
	Log::Info(L"Index: %d, position: %f %f %f, speed: %f", index, position[0], position[1], position[2], speed);
	
	if(!SafeCheckPlayer(index)) return;

	float lx,ly,lz;
	Natives::GetCharCoordinates(gPlayer[index].PedID, &lx, &ly, &lz);

	float d = GetDist(lx, ly, lz, position[0], position[1], position[2]);
	if(gPlayer[index].vehicleindex == -1) // Если пешком
	{
		Log::Info(L"MovePlayer (HOOK) %s", "Start");
		int ms = 4;
		Log::Info(L"Before Move");
		if(speed < 3)
		{
			ms = 2;
			if(d > 10)
			{
				Natives::SetCharCoordinates(gPlayer[index].PedID, position[0], position[1], position[2]);
				Natives::SetCharHeading(gPlayer[index].PedID, gPlayer[index].angle);
				Log::Info(L"PORTAL or TIMESHIFT");
			}
		}
		else if(speed < 5)
		{
			ms = 3;
			if(d > 20)
			{
				Natives::SetCharCoordinates(gPlayer[index].PedID, position[0], position[1], position[2]);
				Natives::SetCharHeading(gPlayer[index].PedID, gPlayer[index].angle);
				Log::Info(L"PORTAL or TIMESHIFT");
			}
		}
		else
		{
			ms = 4;
			if(d > 30)
			{
				Natives::SetCharCoordinates(gPlayer[index].PedID, position[0], position[1], position[2]);
				Natives::SetCharHeading(gPlayer[index].PedID, gPlayer[index].angle);
				Log::Info(L"PORTAL or TIMESHIFT");
			}
		}

		Natives::TaskGoStraightToCoord(gPlayer[index].PedID, position[0], position[1], position[2], ms, 45000);
		wait(1);
		memcpy(gPlayer[index].position, position, sizeof(float) * 3);
	}
	else // Если на машине
	{
		if(!SafeCheckVehicle(gPlayer[index].vehicleindex)) return;

		int vect = 1;
		if(speed < 0) { vect = 2; speed = speed*-1; }
		if(speed * 3 < d)
		{
			Natives::SetCharCoordinates(gPlayer[index].PedID, position[0], position[1], position[2]);
			Natives::SetCarHeading(gVehicle[gPlayer[index].vehicleindex].CarID, gVehicle[gPlayer[index].vehicleindex].angle);
		}

		Natives::TaskCarDriveToCoord(gPlayer[index].PedID, gVehicle[gPlayer[index].vehicleindex].CarID, position[0], position[1], position[2], speed, vect, 2, 3, 2, 45000000);
	}
	gPlayer[index].last_active = GetTickCount();
}

void FMPHook::PlayerDisconnect(short index)
{
	if(index == client.GetIndex())
	{
		SafeCleanPlayers();
		SafeCleanVehicles();

		client.SetIndex(-1);

		client.SetGameState(GameStateOffline);
		client.SetInputState(InputStateGui);
		return;
	}

	if(!SafeCheckPlayer(index)) return;

	Log::Info(L"DELETE CHAR %d", index);
	Natives::RemoveBlip(gPlayer[index].radarblip);
	Natives::DeleteChar(&gPlayer[index].PedID);
	gPlayer[index].connected = 0;
}

void FMPHook::StartEnterInVehicle(const short playerindex, const short vehicleindex, const char seatindex)
{
	if(!SafeCheckPlayer(playerindex)) return;
	if(!SafeCheckVehicle(vehicleindex)) return;

	if(seatindex == 0) Natives::TaskEnterCarAsDriver(gPlayer[playerindex].PedID, gVehicle[vehicleindex].CarID, -1);
	else if(seatindex > 0) Natives::TaskEnterCarAsPassenger(gPlayer[playerindex].PedID, gVehicle[vehicleindex].CarID, -1, seatindex - 1);
	gPlayer[playerindex].vehicleindex = vehicleindex;
	gPlayer[playerindex].seatindex = seatindex;
}

void FMPHook::CancelEnterInVehicle(const short index)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info(L"CancelEnterInVehicle %d", index);
	Natives::ClearCharTasks(gPlayer[index].PedID);
	gPlayer[index].vehicleindex = -1;
}

void FMPHook::FinishEnterInVehicle(const short index)
{
	if(!SafeCheckPlayer(index)) return;

	if(gPlayer[index].seatindex == 0) Natives::WarpCharIntoCar(gPlayer[index].PedID, gVehicle[gPlayer[index].vehicleindex].CarID);
	else if(gPlayer[index].seatindex > 0) Natives::WarpCharIntoCarAsPassenger(gPlayer[index].PedID, gVehicle[gPlayer[index].vehicleindex].CarID, gPlayer[index].seatindex - 1);
}

void FMPHook::StartExitFromVehicle(const short index)
{
	if(!SafeCheckPlayer(index)) return;
	if(!SafeCheckVehicle(gPlayer[index].vehicleindex)) return;

	Log::Info(L"StartExitFromVehicle %d", index);
	Natives::TaskLeaveAnyCar(gPlayer[index].PedID);
}

void FMPHook::FinishExitFromVehicle(const short index)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info(L"FinishExitFromVehicle %d", index);
	
	Natives::SetCharCoordinates(gPlayer[index].PedID, gPlayer[index].position[0], gPlayer[index].position[1], gPlayer[index].position[2]);
	gPlayer[index].vehicleindex = INVALID_VEHICLE_INDEX;
}

void FMPHook::PlayerRecieveWeapon(const short index, const eWeapon weapon, const short ammo)
{
	Log::Debug(L"Recieving weapon %d %d", weapon, ammo);
	if(!SafeCheckPlayer(index)) return;
	eWeaponSlot slot;
	switch (weapon)
	{
	case WEAPON_UNARMED:
		{
			return;
			break;
		}
	case WEAPON_BASEBALLBAT:
	case WEAPON_KNIFE:
		{
			slot = WEAPON_SLOT_MELEE;
			break;
		}
	case WEAPON_GRENADE:
	case WEAPON_MOLOTOV:
		{
			slot = WEAPON_SLOT_THROWN;
			break;
		}
	case WEAPON_PISTOL:
	case WEAPON_DEAGLE:
		{
			slot = WEAPON_SLOT_HANDGUN;
			break;
		}
	case WEAPON_SHOTGUN:
	case WEAPON_BARETTA:
		{
			slot = WEAPON_SLOT_SHOTGUN;
			break;
		}
	case WEAPON_MICRO_UZI:
	case WEAPON_MP5:
		{
			slot = WEAPON_SLOT_SMG;
			break;
		}
	case WEAPON_AK47:
	case WEAPON_M4:
		{
			slot = WEAPON_SLOT_RIFLE;
			break;
		}
	case WEAPON_SNIPERRIFLE:
	case WEAPON_M40A1:
		{
			slot = WEAPON_SLOT_SNIPER;
			break;
		}
	case WEAPON_RLAUNCHER:
		{
			slot = WEAPON_SLOT_HEAVY;
			break;
		}
	default:
		{
			return;
			break;
		}
	}
	gPlayer[index].weapons[slot] = weapon;
	gPlayer[index].ammo[slot] = ammo;
	Natives::GiveWeaponToChar(gPlayer[index].PedID, weapon, ammo, 0);
	Natives::SetCharAmmo(gPlayer[index].PedID, weapon, ammo);
	Log::Debug(L"Recieved weapon %d %d", weapon, ammo);
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

void FMPHook::SetPosition(short index, float pos[3], float angle)
{
	if(!SafeCheckPlayer(index)) return;

	memcpy(gPlayer[index].position, pos, 3 * sizeof(float));
	gPlayer[index].angle = angle;

	Natives::SetCharCoordinates(gPlayer[index].PedID, pos[0], pos[1], pos[2]);
	Natives::SetCharHeading(gPlayer[index].PedID, angle);
}

void FMPHook::PlayerSyncSkin(short index, unsigned int skin)
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info(L"Player Sync Skin START");
	if(index == client.GetIndex())
	{
		SafeChangeModel(skin);
	}
	else
	{
		Natives::DeleteChar(&gPlayer[index].PedID);
		gPlayer[index].model = skin;
		SafeCreatePlayer(index);
	}
	Log::Info(L"Player Sync Skin END");
}

void FMPHook::PlayerSyncSkinVariation(short index, int sm[11], int st[11])
{
	if(!SafeCheckPlayer(index)) return;

	Log::Info(L"Player Sync Skin Variation START");
	for(int i = 0; i < 11; i++)
		Natives::SetCharComponentVariation(gPlayer[index].PedID, (ePedComponent)i, sm[i], st[i]); 
	Log::Info(L"Player Sync Skin Variantion END");
}

void FMPHook::xPlayerSpawn(NetworkPlayerSpawnData data)
{
	Log::Info(L"Spawn %d (%d)", data.client, client.GetIndex());
	if(data.client == client.GetIndex()) 
	{
		Log::Info(L"Its me");
		if(gPlayer[client.GetIndex()].sync_state == 1) SetFreeCam(0);
		gPlayer[client.GetIndex()].sync_state = 0;
		return;
	}
	if(!SafeCheckPlayer(data.client)) 
	{
		return;
	}
	Log::Info(L"Player Spawn START");

	Ped old = gPlayer[data.client].PedID;
	
	SafeRequestModel(data.model);
	Natives::CreateChar(1, (eModel)data.model, data.position[0], data.position[1], data.position[2], &gPlayer[data.client].PedID, 1);

	while(!Natives::DoesCharExist(gPlayer[data.client].PedID)) wait(0);

	Natives::SetCharDefaultComponentVariation(gPlayer[data.client].PedID);
	PlayerSyncSkinVariation(data.client, data.compD, data.compT);

	Log::Info(L"Set UP");
	Natives::SetCharHeading(gPlayer[data.client].PedID, data.position[3]);
	Natives::SetCharHealth(gPlayer[data.client].PedID, data.health);
	Natives::AddArmourToChar(gPlayer[data.client].PedID, data.armor);
	Natives::SetCharInvincible(gPlayer[data.client].PedID, 1);
	Natives::SetBlockingOfNonTemporaryEvents(gPlayer[data.client].PedID, 1);

	if(data.room != 0) Natives::SetRoomForCharByKey(gPlayer[data.client].PedID, (eInteriorRoomKey)data.room);
	Log::Info(L"Player SPAWN END");
	if(Natives::DoesCharExist(old)) Natives::DeleteChar(&old);
	Log::Info(L"Player Old Delete END");
}