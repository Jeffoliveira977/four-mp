/*
*	Created:			29.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/
#include "main.h"
#include "log.h"
#include "Hook\types.h"
#include "Hook\classes.h"
#include "structs.h"

#include "net\RakNetworkFactory.h"
#include "net\RakPeerInterface.h"
#include "net\MessageIdentifiers.h"
#include "net\BitStream.h"

extern FMPHook HOOK;
extern int LastUpdate;
extern int MyID;
extern RakPeerInterface *net;

extern FPlayer gPlayer[MAX_PLAYERS];
extern FVehicle gCar[MAX_CARS];


void ErrorConnect(RPCParameters *rpcParameters)
{
	Log("ErrorConnect\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int error;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(error);

	switch(error)
	{
	case 1:
		{
		// ERROR 1 - Имя уже существует
			Log("ConnectError: %d Name already exist", error);
		} break;
	case 2:
		{
		// ERROR 2 - НЕТ МЕСТ
			Log("ConnectError: %d Free slots is NULL", error);
		} break;
	case 3:
		{
		// ERROR 3 - Скипт не пускает
			Log("ConnectError: %d Script lock connect", error);
		} break;
	default:
		break;
	}
}

void ConnectPlayer(RPCParameters *rpcParameters)
{
	Log("ConnectPlayer Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	char name[32];
	int index, size = 0;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(size);
	bsData.Read(name, size);
	bsData.Read(index);
	bsData.Read(gPlayer[index].model);
	bsData.Read(gPlayer[index].x);
	bsData.Read(gPlayer[index].y);
	bsData.Read(gPlayer[index].z);

	bsData.Read(gPlayer[index].angle);
	bsData.Read(gPlayer[index].car_id);
	bsData.Read(gPlayer[index].seat_id);
	bsData.Read(gPlayer[index].score);
	bsData.Read(gPlayer[index].health);
	bsData.Read(gPlayer[index].armour);
	bsData.Read(gPlayer[index].Room);

	bsData.Read(gPlayer[index].gWeapons[0]);
	bsData.Read(gPlayer[index].gAmmo[0]);
	bsData.Read(gPlayer[index].gWeapons[1]);
	bsData.Read(gPlayer[index].gAmmo[1]);
	bsData.Read(gPlayer[index].gWeapons[2]);
	bsData.Read(gPlayer[index].gAmmo[2]);
	bsData.Read(gPlayer[index].gWeapons[3]);
	bsData.Read(gPlayer[index].gAmmo[3]);
	bsData.Read(gPlayer[index].gWeapons[4]);
	bsData.Read(gPlayer[index].gAmmo[4]);
	bsData.Read(gPlayer[index].gWeapons[5]);
	bsData.Read(gPlayer[index].gAmmo[5]);
	bsData.Read(gPlayer[index].gWeapons[6]);
	bsData.Read(gPlayer[index].gAmmo[6]);
	bsData.Read(gPlayer[index].gWeapons[7]);
	bsData.Read(gPlayer[index].gAmmo[7]);

	bsData.Read(gPlayer[index].color, 4);

	name[size] = '\0';
	Log("ConnectPlayer Center %s\r\n", name);
	HOOK.PlayerConnect(name, index, gPlayer[index].model, gPlayer[index].x, gPlayer[index].y, gPlayer[index].z);
	Log("ConnectPlayer End\r\n");
}

void MovePlayer(RPCParameters *rpcParameters)
{
	Log("MovePlayer Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid;
	float x,y,z,a,s;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);
	bsData.Read(x);
	bsData.Read(y);
	bsData.Read(z);
	bsData.Read(a);
	bsData.Read(s);

	if(gPlayer[playerid].car_id != -1)
	{
		gCar[gPlayer[playerid].car_id].x = x;
		gCar[gPlayer[playerid].car_id].y = y;
		gCar[gPlayer[playerid].car_id].z = z;
		gCar[gPlayer[playerid].car_id].angle = a;
	}

	gPlayer[playerid].x = x;
	gPlayer[playerid].y = y;
	gPlayer[playerid].z = z;
	gPlayer[playerid].angle = a;

	Log("MovePlayer Center (%d,%f,%f,%f)\r\n", playerid, x, y, z);
	HOOK.PlayerMove(playerid, x, y, z, s);
	Log("MovePlayer End\r\n");
}

void JumpPlayer(RPCParameters *rpcParameters)
{
	Log("JumpPlayer Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);

	Log("Jumplayer Center (%d)\r\n", playerid);
	HOOK.Jump(playerid);
	Log("JumpPlayer End\r\n");
}

void DuckPlayer(RPCParameters *rpcParameters)
{
	Log("DuckPlayer Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid;
	bool duck;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);
	bsData.Read(duck);
	Log("DuckPlayer Center (%d,%d)\r\n", playerid, duck);
	HOOK.Duck(playerid, duck);
	Log("DuckPlayer End\r\n");
}

void Check(RPCParameters *rpcParameters)
{
	Log("Check Start\r\n");
	LastUpdate = GetTickCount();
	
	RakNet::BitStream bsSend;
	bsSend.Write(MyID);
	net->RPC("Check",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		
	Log("Check End\r\n");
}

void Disconnect(RPCParameters *rpcParameters)
{
	Log("Disconnect Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int id;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(id);
	HOOK.PlayerDisconnect(id);
	Log("MovePlayer End\r\n");
}


void CreateVehicle(RPCParameters *rpcParameters)
{
	Log("CreateVehicle Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int car, model, c1, c2;
	float x,y,z,r;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(car);
	bsData.Read(model);
	bsData.Read(x);
	bsData.Read(y);
	bsData.Read(z);
	bsData.Read(r);
	bsData.Read(c1);
	bsData.Read(c2);
	Log("CreateVehicle Center (%d,%f,%f,%f)\r\n", car, x, y, z);
	HOOK.CreateCar(car, model, x, y, z, r, c1, c2);
	Log("CreateVehicle End\r\n");
}

void PlayerCancelEnterInVehicle(RPCParameters *rpcParameters)
{
	Log("PlayerCancelEnterInVehicle Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);

	Log("PlayerCancelEnterInVehicle Center (%d)\r\n", playerid);
	HOOK.CancelEnterInVehicle(playerid);
	Log("PlayerCancelEnterInVehicle End\r\n");
}

void PlayerExitFromVehicle(RPCParameters *rpcParameters)
{
	Log("PlayerExitFromVehicle Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);

	Log("PlayerExitFromVehicle Center (%d)\r\n", playerid);
	HOOK.ExitFromVehicle(playerid);
	Log("PlayerExitFromVehicle End\r\n");
}

void PlayerEnterInVehicle(RPCParameters *rpcParameters)
{
	Log("PlayerEnterInVehicle Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid, car, seat;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);
	bsData.Read(car);
	bsData.Read(seat);

	Log("PlayerEnterInVehicle Center (%d,%d)\r\n", playerid, car);
	HOOK.EnterInVehicle(playerid, car, seat);
	Log("PlayerEnterInVehicle End\r\n");
}



void PlayerFire(RPCParameters *rpcParameters)
{
	Log("PlayerFire Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid, gun, time;
	float x, y, z;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);
	bsData.Read(x);
	bsData.Read(y);
	bsData.Read(z);
	bsData.Read(gun);
	bsData.Read(time);

	Log("PlayerFire Center (%d,%d)\r\n", playerid, gun);
	HOOK.PlayerFireAim(playerid, gun, time, x, y, z, 1);
	Log("PlayerFire End\r\n");
}

void PlayerAim(RPCParameters *rpcParameters)
{
	Log("PlayerAim Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid, gun, time;
	float x, y, z;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);
	bsData.Read(x);
	bsData.Read(y);
	bsData.Read(z);
	bsData.Read(gun);
	bsData.Read(time);

	Log("PlayerAim Center (%d,%d)\r\n", playerid, gun);
	HOOK.PlayerFireAim(playerid, gun, time, x, y, z, 0);
	Log("PlayerAim End\r\n");
}

void SwapGun(RPCParameters *rpcParameters)
{
	Log("SwapGun Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid, gun;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);
	bsData.Read(gun);

	Log("SwapGun Center (%d,%d)\r\n", playerid, gun);
	HOOK.PlayerSwapGun(playerid, gun);
	Log("SwapGun End\r\n");
}

void PlayerParams(RPCParameters *rpcParameters)
{
	Log("PlayerParams Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid, hp, arm;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);
	bsData.Read(hp);
	bsData.Read(arm);

	// ... Do it ...
	Log("PlayerParams End\r\n");
}

void SyncSkin(RPCParameters *rpcParameters)
{
	Log("SyncSkin Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid, skin;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);
	bsData.Read(skin);

	Log("SyncSkin Center (%d,%d)\r\n", playerid, skin);
	HOOK.PlayerSyncSkin(playerid, skin);
	Log("SyncSkin End\r\n");
}

void SyncSkinVariation(RPCParameters *rpcParameters)
{
	Log("SyncSkinVariation Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid, sm[11], st[11];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);
	bsData.Read(sm);
	bsData.Read(st);

	Log("SyncSkinVariation Center (%d,%d)\r\n", playerid, sm[0]);
	HOOK.PlayerSyncSkinVariation(playerid, sm, st);
	Log("SyncSkinVariation End\r\n");
}

void ClassSync(RPCParameters *rpcParameters)
{
	Log("ClassSync Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(Conf.ComponentSelect);
	bsData.Read(Conf.NumSkins);
	for(int i = 0; i < Conf.NumSkins; i++)
		bsData.Read(pClass[i]);
	Log("ClassSync Center\r\n");
	Conf.SkinSelect = 1; 
	Log("ClassSync End\r\n");
}

void PlayerSpawn(RPCParameters *rpcParameters)
{
	Log("PlayerSpawn Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int pid;
	SpawnInfo spawn;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(pid);
	bsData.Read(spawn);

	Log("PlayerSpawn Center\r\n");
	HOOK.PlayerSpawn(pid, spawn);
	Log("PlayerSpawn End\r\n");
}