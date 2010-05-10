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
#include "rpc.h"
#include "chat.h"
#include "..\..\Shared\Network\NetworkProtocol.h"

#include "..\..\Shared\RakNet\RakNetworkFactory.h"
#include "..\..\Shared\RakNet\RakPeerInterface.h"
#include "..\..\Shared\RakNet\MessageIdentifiers.h"
#include "..\..\Shared\RakNet\BitStream.h"

extern FMPHook HOOK;
extern int LastUpdate;
extern short MyID;
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

	NetworkPlayerConnectionErrorData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);

	switch (data.error)
	{
	case NetworkPlayerConnectionErrorServerFull:
		{
			// НЕТ МЕСТ
			Log("ConnectError: Server is full.");
			break;
		}
	case NetworkPlayerConnectionErrorAlreadyConnected:
		{
			// Уже присоединён
			Log("ConnectError: You are already connected.");
			break;
		}
	case NetworkPlayerConnectionErrorAllocationError:
		{
			// Ошибка сервера
			Log("ConnectError: Server was unable to allocate player resources.");
			break;
		}
	case NetworkPlayerConnectionErrorScriptLock:
		{
			// Скипт не пускает
			Log("ConnectError: Script lock connect");
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
	gPlayer[data.index].seatindex = data.seatindex;
	gPlayer[data.index].score = data.score;
	gPlayer[data.index].health = data.health;
	gPlayer[data.index].armor = data.armor;
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

	NetworkPlayerMoveData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);
	if(gPlayer[data.client].vehicleindex != -1)
	{
		memcpy(gCar[gPlayer[data.client].vehicleindex].position, data.position, sizeof(float) * 3);
		gCar[gPlayer[data.client].vehicleindex].angle = data.angle;
	}

	memcpy(gPlayer[data.client].position, data.position, sizeof(float) * 3);
	gPlayer[data.client].angle = data.angle;

	HOOK.PlayerMove(data.client, data.position[0], data.position[1], data.position[2], data.speed);
	Log("MovePlayer End");
}

void JumpPlayer(RPCParameters *rpcParameters)
{
	Log("JumpPlayer Start");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerJumpData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);

	Log("Jumplayer Center (%d)", data.client);
	HOOK.Jump(data.client);
	Log("JumpPlayer End");
}

void DuckPlayer(RPCParameters *rpcParameters)
{
	Log("DuckPlayer Start");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerDuckData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);
	Log("DuckPlayer Center (%d,%d)", data.client, data.shouldduck);
	HOOK.Duck(data.client, data.shouldduck);
	Log("DuckPlayer End");
}

void Check(RPCParameters *rpcParameters)
{
	Log("Check Start");
	LastUpdate = GetTickCount();
	
	NetworkPlayerCheckData data;
	data.client = MyID;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
	net->RPC("RPC_Check",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		
	Log("Check End");
}

void Disconnect(RPCParameters *rpcParameters)
{
	Log("Disconnect Start");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerDisconnectionData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);
	HOOK.PlayerDisconnect(data.client);
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

	NetworkPlayerCancelEntranceInVehicleData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);

	Log("PlayerCancelEnterInVehicle Center (%d)\r\n", data.client);
	HOOK.CancelEnterInVehicle(data.client);
	Log("PlayerCancelEnterInVehicle End\r\n");
}

void PlayerExitFromVehicle(RPCParameters *rpcParameters)
{
	Log("PlayerExitFromVehicle Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerExitFromVehicleData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);

	Log("PlayerExitFromVehicle Center (%d)\r\n", data.client);
	HOOK.ExitFromVehicle(data.client);
	Log("PlayerExitFromVehicle End\r\n");
}

void PlayerEnterInVehicle(RPCParameters *rpcParameters)
{
	Log("PlayerEnterInVehicle Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerEntranceInVehicleData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);

	Log("PlayerEnterInVehicle Center (%d,%d)\r\n", data.client, data.vehicleindex);
	HOOK.EnterInVehicle(data.client, data.vehicleindex, data.seat);
	Log("PlayerEnterInVehicle End\r\n");
}

void PlayerFire(RPCParameters *rpcParameters)
{
	Log("PlayerFire Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerFireData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);

	Log("PlayerFire Center (%d,%d)\r\n", data.client, data.weapon);
	HOOK.PlayerFireAim(data.client, data.weapon, data.time, data.position[0], data.position[1], data.position[2], 1);
	Log("PlayerFire End\r\n");
}

void PlayerAim(RPCParameters *rpcParameters)
{
	Log("PlayerAim Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerAimData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);
	
	Log("PlayerAim Center (%d,%d)\r\n", data.client, data.weapon);
	HOOK.PlayerFireAim(data.client, data.weapon, data.time, data.position[0], data.position[1], data.position[2], 0);
	Log("PlayerAim End\r\n");
}

void SwapGun(RPCParameters *rpcParameters)
{
	Log("SwapGun Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerWeaponChangeData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);

	Log("SwapGun Center (%d,%d)\r\n", data.client, data.weapon);
	HOOK.PlayerSwapGun(data.client, data.weapon);
	Log("SwapGun End\r\n");
}

void PlayerParams(RPCParameters *rpcParameters)
{
	Log("PlayerParams Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerHealthAndArmorChangeData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);
	
	// ... Do it ...
	Log("PlayerParams End\r\n");
}

void SyncSkin(RPCParameters *rpcParameters)
{
	Log("SyncSkin Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerModelChangeData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);

	Log("SyncSkin Center (%d,%d)\r\n", data.client, data.model);
	HOOK.PlayerSyncSkin(data.client, data.model);
	Log("SyncSkin End\r\n");
}

void SyncSkinVariation(RPCParameters *rpcParameters)
{
	Log("SyncSkinVariation Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerComponentsChangeData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);

	HOOK.PlayerSyncSkinVariation(data.client, data.compD, data.compT);
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

	NetworkPlayerSpawnData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);

	Log("PlayerSpawn Center\r\n");
	HOOK.xPlayerSpawn(data);
	Log("PlayerSpawn End\r\n");
}

void Chat(RPCParameters *rpcParameters)
{
	Log("Chat Start\r\n");
	LastUpdate = GetTickCount();
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;

	NetworkPlayerChatData data;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(data);
	Log("Chat Center\r\n");
	AddChatMessage(data.msg, COLOR(data.color[0], data.color[1], data.color[2]), data.client);
	Log("Chat End\r\n");
}