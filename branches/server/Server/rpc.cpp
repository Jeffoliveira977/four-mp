/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/
#include <windows.h>
// RakNet
#include "net\RakNetworkFactory.h"
#include "net\RakPeerInterface.h"
#include "net\MessageIdentifiers.h"
#include "net\BitStream.h"
// Other
#include "main.h"
#include "manager.h"

extern RakPeerInterface *net;
/*
-- RPC HELP --
void RPCFUNC(RPCParameters *rpcParameters)

unsigned char* Data = rpcParameters->input; 
int iBitLength = rpcParameters->numberOfBitsOfData;
SystemAddress dender = rpcParameters->sender;

RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
bsData.Read(VAR);

RakNet::BitStream bsSend;
bsSend.Write(PARAM);
net->RPC("FUNC",&bsSend,HIGH_PRIORITY,RELIABLE,0,sender,false, 0, UNASSIGNED_NETWORK_ID, 0);
*/

void ClientConnect(RPCParameters *rpcParameters)
{
	debug("ClientConnect Start");
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;

	int size = 0;
	char name[32];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(size);
	bsData.Read(name, size);
	name[size] = '\0';
	debug("ClientConnect FOR 1 %s[%d]", name, size);
	for(int i=0; i < MAX_PLAYERS; i++)
	{
		if(gPlayer[i].connected == 1 && strcmp(gPlayer[i].name, name) == 0)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(1);
			net->RPC("ErrorConnect",&bsSend,HIGH_PRIORITY,RELIABLE,0,sender,false, 0, UNASSIGNED_NETWORK_ID, 0);
			return;
		}
	}
	debug("ClientConnect FOR 2");
	for(int index=0; index < MAX_PLAYERS; index++)
	{
		if(gPlayer[index].connected != 1)
		{
			print("New player %s[%d] connected", name, index);

			int r = PlayerConnect(name, index);
			if(r != 0)
			{
				RakNet::BitStream bsSend;
				bsSend.Write(r);
				net->RPC("ErrorConnect",&bsSend,HIGH_PRIORITY,RELIABLE,0,sender,false, 0, UNASSIGNED_NETWORK_ID, 0);
			}

			gPlayer[index].connected = 1;

			strcpy(gPlayer[index].ip, sender.ToString(0));
			gPlayer[index].port = sender.port;
			gPlayer[index].sa = sender;
			strcpy(gPlayer[index].name, name);

			RakNet::BitStream bsSend;
			bsSend.Write(strlen(gPlayer[index].name));
			bsSend.Write(gPlayer[index].name,strlen(gPlayer[index].name));
			bsSend.Write(index);
			bsSend.Write(gPlayer[index].model);
			bsSend.Write(gPlayer[index].x);
			bsSend.Write(gPlayer[index].y);
			bsSend.Write(gPlayer[index].z);

			bsSend.Write(gPlayer[index].angle);
			bsSend.Write(gPlayer[index].car_id);
			bsSend.Write(gPlayer[index].seat_id);
			bsSend.Write(gPlayer[index].score);
			bsSend.Write(gPlayer[index].health);
			bsSend.Write(gPlayer[index].armour);
			bsSend.Write(gPlayer[index].Room);

			bsSend.Write(gPlayer[index].gWeapons[0]);
			bsSend.Write(gPlayer[index].gAmmo[0]);
			bsSend.Write(gPlayer[index].gWeapons[1]);
			bsSend.Write(gPlayer[index].gAmmo[1]);
			bsSend.Write(gPlayer[index].gWeapons[2]);
			bsSend.Write(gPlayer[index].gAmmo[2]);
			bsSend.Write(gPlayer[index].gWeapons[3]);
			bsSend.Write(gPlayer[index].gAmmo[3]);
			bsSend.Write(gPlayer[index].gWeapons[4]);
			bsSend.Write(gPlayer[index].gAmmo[4]);
			bsSend.Write(gPlayer[index].gWeapons[5]);
			bsSend.Write(gPlayer[index].gAmmo[5]);
			bsSend.Write(gPlayer[index].gWeapons[6]);
			bsSend.Write(gPlayer[index].gAmmo[6]);
			bsSend.Write(gPlayer[index].gWeapons[7]);
			bsSend.Write(gPlayer[index].gAmmo[7]);

			bsSend.Write(gPlayer[index].color);

			debug("ClientConnect ME SEND TO %s:%d  - %d", gPlayer[index].ip, gPlayer[index].port, gPlayer[index].model);
			bool rs = net->RPC("ConnectPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[index].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
			printf("%d", rs);
			debug("ClientConnect ME END");

			for(int i = 0; i < MAX_PLAYERS; i++)
			{
				if(gPlayer[i].connected == 1 && i != index)
				{
					debug("+++ %d != %d | %d", i, index, gPlayer[i].connected);

					bsSend.Reset();
					bsSend.Write(strlen(gPlayer[index].name));
					bsSend.Write(gPlayer[index].name,strlen(gPlayer[index].name));
					bsSend.Write(index);
					bsSend.Write(gPlayer[index].model);
					bsSend.Write(gPlayer[index].x);
					bsSend.Write(gPlayer[index].y);
					bsSend.Write(gPlayer[index].z);

					bsSend.Write(gPlayer[index].angle);
					bsSend.Write(gPlayer[index].car_id);
					bsSend.Write(gPlayer[index].seat_id);
					bsSend.Write(gPlayer[index].score);
					bsSend.Write(gPlayer[index].health);
					bsSend.Write(gPlayer[index].armour);
					bsSend.Write(gPlayer[index].Room);

					bsSend.Write(gPlayer[index].gWeapons[0]);
					bsSend.Write(gPlayer[index].gAmmo[0]);
					bsSend.Write(gPlayer[index].gWeapons[1]);
					bsSend.Write(gPlayer[index].gAmmo[1]);
					bsSend.Write(gPlayer[index].gWeapons[2]);
					bsSend.Write(gPlayer[index].gAmmo[2]);
					bsSend.Write(gPlayer[index].gWeapons[3]);
					bsSend.Write(gPlayer[index].gAmmo[3]);
					bsSend.Write(gPlayer[index].gWeapons[4]);
					bsSend.Write(gPlayer[index].gAmmo[4]);
					bsSend.Write(gPlayer[index].gWeapons[5]);
					bsSend.Write(gPlayer[index].gAmmo[5]);
					bsSend.Write(gPlayer[index].gWeapons[6]);
					bsSend.Write(gPlayer[index].gAmmo[6]);
					bsSend.Write(gPlayer[index].gWeapons[7]);
					bsSend.Write(gPlayer[index].gAmmo[7]);

					bsSend.Write(gPlayer[index].color);

					debug("ClientConnect SEND TO %s:%d  - %d", gPlayer[i].ip, gPlayer[i].port, gPlayer[index].model);
					net->RPC("ConnectPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
					debug("ClientConnect END");

					bsSend.Reset();
					bsSend.Write(strlen(gPlayer[i].name));
					bsSend.Write(gPlayer[i].name,strlen(gPlayer[i].name));
					bsSend.Write(i);
					bsSend.Write(gPlayer[i].model);
					bsSend.Write(gPlayer[i].x);
					bsSend.Write(gPlayer[i].y);
					bsSend.Write(gPlayer[i].z);

					bsSend.Write(gPlayer[i].angle);
					bsSend.Write(gPlayer[i].car_id);
					bsSend.Write(gPlayer[i].seat_id);
					bsSend.Write(gPlayer[i].score);
					bsSend.Write(gPlayer[i].health);
					bsSend.Write(gPlayer[i].armour);
					bsSend.Write(gPlayer[i].Room);

					bsSend.Write(gPlayer[i].gWeapons[0]);
					bsSend.Write(gPlayer[i].gAmmo[0]);
					bsSend.Write(gPlayer[i].gWeapons[1]);
					bsSend.Write(gPlayer[i].gAmmo[1]);
					bsSend.Write(gPlayer[i].gWeapons[2]);
					bsSend.Write(gPlayer[i].gAmmo[2]);
					bsSend.Write(gPlayer[i].gWeapons[3]);
					bsSend.Write(gPlayer[i].gAmmo[3]);
					bsSend.Write(gPlayer[i].gWeapons[4]);
					bsSend.Write(gPlayer[i].gAmmo[4]);
					bsSend.Write(gPlayer[i].gWeapons[5]);
					bsSend.Write(gPlayer[i].gAmmo[5]);
					bsSend.Write(gPlayer[i].gWeapons[6]);
					bsSend.Write(gPlayer[i].gAmmo[6]);
					bsSend.Write(gPlayer[i].gWeapons[7]);
					bsSend.Write(gPlayer[i].gAmmo[7]);

					bsSend.Write(gPlayer[i].color);
					debug("+++ ClientConnect SEND TO %s - %d", gPlayer[i].name, gPlayer[i].model);
					net->RPC("ConnectPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[index].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
					debug("+++ ClientConnect END");
				}
			}
			debug("+++--------------------+++");
			for(int i = 0; i < MAX_VEHICLES; i++)
			{
				if(gVehicle[i].exist == 1)
				{
					RakNet::BitStream bsSend;
					bsSend.Write(i);
					bsSend.Write(gVehicle[i].model);
					bsSend.Write(gVehicle[i].x);
					bsSend.Write(gVehicle[i].y);
					bsSend.Write(gVehicle[i].z);
					bsSend.Write(gVehicle[i].angle);
					bsSend.Write(gVehicle[i].color[0]);
					bsSend.Write(gVehicle[i].color[1]);

					net->RPC("CreateVehicle",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[index].sa ,false, 0, UNASSIGNED_NETWORK_ID, 0);
				}
			}
			RakNet::BitStream bSend;
			bSend.Write(sConf.ComponentSelect);
			bSend.Write(sConf.NumSkins);
			for(int j = 0; j < sConf.NumSkins; j++)
				bSend.Write(pClass[j]);

			net->RPC("ClassSync",&bSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[index].sa ,false, 0, UNASSIGNED_NETWORK_ID, 0);
			debug("------------------");
			return;
		}
	}

	RakNet::BitStream bsSend;
	bsSend.Write(2);
	net->RPC("ErrorConnect",&bsSend,HIGH_PRIORITY,RELIABLE,0,sender,false, 0, UNASSIGNED_NETWORK_ID, 0);
}

void Check(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;

	int id;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(id);
}

void MovePlayer(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;

	float x,y,z,a,sp;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(x);
	bsData.Read(y);
	bsData.Read(z);
	bsData.Read(a);
	bsData.Read(sp);

	int index = GetPlayerID(sender);

	if(gPlayer[index].car_id != -1)
	{
		gVehicle[gPlayer[index].car_id].x = x;
		gVehicle[gPlayer[index].car_id].y = y;
		gVehicle[gPlayer[index].car_id].z = z;
		gVehicle[gPlayer[index].car_id].angle = a;
	}

	gPlayer[index].x = x;
	gPlayer[index].y = y;
	gPlayer[index].z = z;
	gPlayer[index].angle = a;

	debug("PlayerMove %d to (%f,%f,%f)", index, x, y, z);

	if(gPlayer[index].car_enter == 1) return;

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i != index && gPlayer[i].connected == 1)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(index);
			bsSend.Write(x);
			bsSend.Write(y);
			bsSend.Write(z);
			bsSend.Write(a);
			bsSend.Write(sp);
			net->RPC("MovePlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void JumpPlayer(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;
	
	int index = GetPlayerID(sender);

	debug("JUMP %d", index);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i != index && gPlayer[i].connected == 1)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(index);
			net->RPC("JumpPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void DuckPlayer(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;

	bool duck;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(duck);

	int index = GetPlayerID(sender);

	gPlayer[index].duck = duck;

	debug("DUCK %d to %d", index, duck);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i != index && gPlayer[i].connected == 1)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(index);
			bsSend.Write(duck);
			net->RPC("DuckPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void PlayerEnterInVehicle(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;

	int car, seat;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(car);
	bsData.Read(seat);

	int index = GetPlayerID(sender);

	if(seat != 100)
	{
		gPlayer[index].car_enter = 1;
		gPlayer[index].seat_id = seat;

		debug("ENTER To CAR %d to %d as %d", index, car, seat);

		for(int i = 0; i < MAX_PLAYERS; i++)
		{
			if(i != index && gPlayer[i].connected == 1)
			{
				RakNet::BitStream bsSend;
				bsSend.Write(index);
				bsSend.Write(car);
				bsSend.Write(seat);
				net->RPC("PlayerEnterInVehicle",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
			}
		}
	}
	else
	{
		gPlayer[index].car_enter = 0;
	}
}

void PlayerCancelEnterInVehicle(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;
	
	int index = GetPlayerID(sender);
	gPlayer[index].car_id = -1;
	gPlayer[index].car_enter = 0;

	debug("PlayerCancelEnterInVehicle %d", index);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i != index && gPlayer[i].connected == 1)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(index);
			net->RPC("PlayerCancelEnterInVehicle",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void PlayerExitFromVehicle(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;
	
	int index = GetPlayerID(sender);
	gPlayer[index].car_id = -1;
	gPlayer[index].car_enter = 0;

	debug("PlayerExitFromVehicle %d", index);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i != index && gPlayer[i].connected == 1)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(index);
			net->RPC("PlayerExitFromVehicle",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void PlayerFire(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;

	float x, y, z;
	int gun, time;
	PlayerDamage dam;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(x);
	bsData.Read(y);
	bsData.Read(z);
	bsData.Read(gun);
	bsData.Read(time);
	bsData.Read(dam);

	int index = GetPlayerID(sender);

	gPlayer[index].gun = gun;

	if(dam.pid != -1)
	{
		gPlayer[dam.pid].health = dam.hp;
		gPlayer[dam.pid].armour = dam.armour;
	}

	debug("FIRE %d to %d", index, gun);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i != index && gPlayer[i].connected == 1)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(index);
			bsSend.Write(x);
			bsSend.Write(y);
			bsSend.Write(z);
			bsSend.Write(gun);
			bsSend.Write(time);
			net->RPC("PlayerFire",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void PlayerAim(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;

	float x, y, z;
	int gun, time;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(x);
	bsData.Read(y);
	bsData.Read(z);
	bsData.Read(gun);
	bsData.Read(time);

	int index = GetPlayerID(sender);

	gPlayer[index].gun = gun;

	debug("AIM %d to %d", index, gun);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i != index && gPlayer[i].connected == 1)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(index);
			bsSend.Write(x);
			bsSend.Write(y);
			bsSend.Write(z);
			bsSend.Write(gun);
			bsSend.Write(time);
			net->RPC("PlayerAim",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void SwapGun(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;

	int gun;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(gun);

	int index = GetPlayerID(sender);

	gPlayer[index].gun = gun;

	debug("SWAP %d to %d", index, gun);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i != index && gPlayer[i].connected == 1)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(index);
			bsSend.Write(gun);
			net->RPC("SwapGun",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void PlayerParams(RPCParameters *rpcParameters)
{
	unsigned char* Data = rpcParameters->input; 
	int iBitLength = rpcParameters->numberOfBitsOfData;
	SystemAddress sender = rpcParameters->sender;

	int hp, arm;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(hp);
	bsData.Read(arm);

	int index = GetPlayerID(sender);

	gPlayer[index].health = hp;
	gPlayer[index].armour = arm;

	debug("PlayerParams %d to %d", index, hp);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(i != index && gPlayer[i].connected == 1)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(index);
			bsSend.Write(hp);
			bsSend.Write(arm);
			net->RPC("PlayerParams",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void PlayerSpawn(RPCParameters *rpcParameters)
{
	debug("PlayerSpawn");
}
void Select_ModelChanged(RPCParameters *rpcParameters)
{
	debug("ModelChanged [select]");
}
void SyncSkinVariation(RPCParameters *rpcParameters)
{
	debug("SyncSkinVariation");
}