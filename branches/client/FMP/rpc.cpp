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
#include "chat.h"
#include "..\..\Shared\Network\NetworkProtocol.h"

#include "..\..\Shared\RakNet\RakNetworkFactory.h"
#include "..\..\Shared\RakNet\RakPeerInterface.h"
#include "..\..\Shared\RakNet\MessageIdentifiers.h"
#include "..\..\Shared\RakNet\BitStream.h"

extern FMPHook HOOK;
extern int LastUpdate;
extern int MyID;
extern RakPeerInterface *net;

extern FPlayer gPlayer[MAX_PLAYERS];
extern FVehicle gCar[MAX_CARS];

void RegisterRPC()
{
	Log("Register RPC");
	REGISTER_STATIC_RPC(net, ConnectPlayer);
	REGISTER_STATIC_RPC(net, Disconnect);

	REGISTER_STATIC_RPC(net, Check);
	REGISTER_STATIC_RPC(net, ErrorConnect);
	REGISTER_STATIC_RPC(net, MovePlayer);
	REGISTER_STATIC_RPC(net, JumpPlayer);
	REGISTER_STATIC_RPC(net, DuckPlayer);

	REGISTER_STATIC_RPC(net, CreateVehicle);

	REGISTER_STATIC_RPC(net, PlayerCancelEnterInVehicle);
	REGISTER_STATIC_RPC(net, PlayerExitFromVehicle);
	REGISTER_STATIC_RPC(net, PlayerEnterInVehicle);

	REGISTER_STATIC_RPC(net, SwapGun);
	REGISTER_STATIC_RPC(net, PlayerFire);
	REGISTER_STATIC_RPC(net, PlayerAim);

	REGISTER_STATIC_RPC(net, PlayerParams);

	REGISTER_STATIC_RPC(net, ClassSync);
	REGISTER_STATIC_RPC(net, SyncSkin);
	REGISTER_STATIC_RPC(net, SyncSkinVariation);
	REGISTER_STATIC_RPC(net, PlayerSpawn);

	REGISTER_STATIC_RPC(net, Chat);
}

void ErrorConnect(RPCParameters *rpcParameters)
{
	Log("ErrorConnect");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int error;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(error);

	switch(error)
	{
	case NetworkPlayerConnectionErrorServerFull:
		{
			// НЕТ МЕСТ
			Log("ConnectError: %d. Server is full.", error);
			break;
		}
	case NetworkPlayerConnectionErrorAlreadyConnected:
		{
			// Уже присоединён
			Log("ConnectError: %d. You are already connected.", error);
			break;
		}
	case NetworkPlayerConnectionErrorAllocationError:
		{
			// Ошибка сервера
			Log("ConnectError: %d. Server was unable to allocate player resources.", error);
			break;
		}
	case NetworkPlayerConnectionErrorScriptLock:
		{
			// Скипт не пускает
			Log("ConnectError: %d Script lock connect", error);
			break;
		}
	}
}

void ConnectPlayer(RPCParameters *rpcParameters)
{
	Log("Recieving server info");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerFullUpdateData data;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);
	gPlayer[data.index].model = data.model;
	memcpy(gPlayer[data.index].position, data.position, sizeof(float) * 3);
	gPlayer[data.index].angle = data.angle;
	gPlayer[data.index].vehicleindex = data.vehicleindex;
	gPlayer[data.index].seat_id = data.seat_id;
	gPlayer[data.index].score = data.score;
	gPlayer[data.index].health = data.health;
	gPlayer[data.index].armour = data.armour;
	gPlayer[data.index].room = data.room;
	memcpy(gPlayer[data.index].weapons, data.weapons, sizeof(int) * 8);
	memcpy(gPlayer[data.index].ammo, data.ammo, sizeof(int) * 8);
	memcpy(gPlayer[data.index].color, data.color, sizeof(unsigned char) * 4);

	Log("ConnectPlayer Center %s", data.name);
	HOOK.PlayerConnect(data.name, data.index, gPlayer[data.index].model, gPlayer[data.index].position[0], gPlayer[data.index].position[1], gPlayer[data.index].position[2]);
	Log("ConnectPlayer End");
}

void MovePlayer(RPCParameters *rpcParameters)
{
	Log("MovePlayer Start");
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

	if(gPlayer[playerid].vehicleindex != -1)
	{
		gCar[gPlayer[playerid].vehicleindex].position[0] = x;
		gCar[gPlayer[playerid].vehicleindex].position[1] = y;
		gCar[gPlayer[playerid].vehicleindex].position[2] = z;
		gCar[gPlayer[playerid].vehicleindex].angle = a;
	}

	gPlayer[playerid].position[0] = x;
	gPlayer[playerid].position[1] = y;
	gPlayer[playerid].position[2] = z;
	gPlayer[playerid].angle = a;

	Log("MovePlayer Center (%d,%f,%f,%f)", playerid, x, y, z);
	HOOK.PlayerMove(playerid, x, y, z, s);
	Log("MovePlayer End");
}

void JumpPlayer(RPCParameters *rpcParameters)
{
	Log("JumpPlayer Start");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);

	Log("Jumplayer Center (%d)", playerid);
	HOOK.Jump(playerid);
	Log("JumpPlayer End");
}

void DuckPlayer(RPCParameters *rpcParameters)
{
	Log("DuckPlayer Start");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int playerid;
	bool duck;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerid);
	bsData.Read(duck);
	Log("DuckPlayer Center (%d,%d)", playerid, duck);
	HOOK.Duck(playerid, duck);
	Log("DuckPlayer End");
}

void Check(RPCParameters *rpcParameters)
{
	Log("Check Start");
	LastUpdate = GetTickCount();
	
	RakNet::BitStream bsSend;
	bsSend.Write(MyID);
	net->RPC("RPC_Check",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		
	Log("Check End");
}

void Disconnect(RPCParameters *rpcParameters)
{
	Log("Disconnect Start");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int id;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(id);
	HOOK.PlayerDisconnect(id);
	Log("MovePlayer End");
}


void CreateVehicle(RPCParameters *rpcParameters)
{
	Log("CreateVehicle Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkVehicleFullUpdateData data;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);
	Log("CreateVehicle Center (%d,%f,%f,%f)\r\n", data.index, data.position[0], data.position[1], data.position[2]);
	HOOK.CreateCar(data.index, data.model, data.position[0], data.position[1], data.position[2], data.angle, data.color[0], data.color[2]);
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
	HOOK.xPlayerSpawn(pid, spawn);
	Log("PlayerSpawn End\r\n");
}

void Chat(RPCParameters *rpcParameters)
{
	Log("Chat Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	int player, len;
	int r, g, b;
	char msg[128];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(player);
	bsData.Read(len);
	bsData.Read(msg);
	bsData.Read(r);
	bsData.Read(g);
	bsData.Read(b);

	msg[len] = 0;

	Log("Chat Center\r\n");
	AddChatMessage(msg, COLOR(r, g, b), player);
	Log("Chat End\r\n");
}