#include "..\..\Shared\RakNet\RakNetworkFactory.h"
#include "..\..\Shared\RakNet\MessageIdentifiers.h"
#include "..\..\Shared\RakNet\BitStream.h"

#include "NetworkManager.h"
#include "logging.h"
#include "rpc.h"
#include "ServerCore.h"
#include "VirtualMachineManager.h"
#include "PlayerManager.h"
#include "VehicleManager.h"

extern ServerCore server;
extern VirtualMachineManager vmm;
extern PlayerManager playm;
extern VehicleManager vm;

NetworkManager::NetworkManager(void)
{
	maxaddressbuffersize = playm.GetMaxPlayers();
	addressbuffersize = 0;
}

NetworkManager::~NetworkManager(void)
{
}

void NetworkManager::Init(const short maxclients, const unsigned short port)
{
	net = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor s(port, 0);
	net->SetMaximumIncomingConnections(maxclients);
	net->Startup(maxclients, 1, &s, 1);
	REGISTER_STATIC_RPC(net, RPC_ClientConnect);
	REGISTER_STATIC_RPC(net, RPC_Check);
	REGISTER_STATIC_RPC(net, RPC_MovePlayer);
	REGISTER_STATIC_RPC(net, RPC_DuckPlayer);
	REGISTER_STATIC_RPC(net, RPC_JumpPlayer);
	REGISTER_STATIC_RPC(net, RPC_PlayerCancelEnterInVehicle);
	REGISTER_STATIC_RPC(net, RPC_PlayerExitFromVehicle);
	REGISTER_STATIC_RPC(net, RPC_PlayerEnterInVehicle);
	REGISTER_STATIC_RPC(net, RPC_PlayerFire);
	REGISTER_STATIC_RPC(net, RPC_PlayerAim);
	REGISTER_STATIC_RPC(net, RPC_SwapGun);
	REGISTER_STATIC_RPC(net, RPC_PlayerParams);
	REGISTER_STATIC_RPC(net, RPC_PlayerSpawn);
	REGISTER_STATIC_RPC(net, RPC_Select_ModelChanged);
	REGISTER_STATIC_RPC(net, RPC_SyncSkinVariation);
	REGISTER_STATIC_RPC(net, RPC_Chat);
}

void NetworkManager::Tick(void)
{
	static Packet *pack;
	bool recheckpacket;
	do
	{
		recheckpacket = false;
		pack = net->Receive();
		if (pack)
		{
			recheckpacket = true;
			switch (pack->data[0])
			{
			case ID_CONNECTION_REQUEST:
				{
					debug("New connection request");
					break;
				}
			case ID_NEW_INCOMING_CONNECTION:
				{
					debug("New connection from %s:%d", pack->systemAddress.ToString(0), pack->systemAddress.port);
					break;
				}
			case (ID_DISCONNECTION_NOTIFICATION || ID_CONNECTION_LOST):
				{
					unsigned char clientindex = this->GetClientIndex(pack->systemAddress);
					if (clientindex != INVALID_PLAYER_INDEX)
					{
						playm.DisconnectPlayer(clientindex);
					}
					break;
				}
			}
			net->DeallocatePacket(pack);
		}
	} while (recheckpacket);
}

void NetworkManager::CheckClients(void)
{
	RakNet::BitStream bsSend;
	this->SendDataToAll("Check", &bsSend);
}

void NetworkManager::DisconnectClient(const short index)
{
	if (index >= addressbuffersize)
	{
		return;
	}
	if (addressbuffer[index] == NULL)
	{
		return;
	}
	free(addressbuffer[index]);
	addressbuffer[index] = NULL;
	RakNet::BitStream bsSend;
	bsSend.Write(index);
	for(unsigned char i = 0; i < addressbuffersize; i++)
	{
		if(addressbuffer[i] != NULL)
		{ 
			net->RPC("Disconnect", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[i][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void NetworkManager::RecieveClientConnection(const RPCParameters *rpcParameters)
{
	if (playm.IsServerFull())
	{
		this->SendConnectionError(rpcParameters->sender, NetworkPlayerConnectionErrorServerFull);
		net->CloseConnection(rpcParameters->sender, true);
		return;
	}
	short clientindex = this->GetClientIndex(rpcParameters->sender);
	if (clientindex != INVALID_PLAYER_INDEX)
	{
		PrintToServer("Player %s has tried to connect twice.", playm.GetPlayerName(clientindex));
		this->SendConnectionError(rpcParameters->sender, NetworkPlayerConnectionErrorAlreadyConnected);
	}
	clientindex = this->RegisterNewClient(rpcParameters->sender);
	if (clientindex == INVALID_PLAYER_INDEX)
	{
		PrintToServer("Unable to register new client.");
		this->SendConnectionError(rpcParameters->sender, NetworkPlayerConnectionErrorAllocationError);
		net->CloseConnection(rpcParameters->sender, true);
		return;
	}
	//char *name;
	//unsigned char namelength = 0;
	NetworkPlayerConnectionRequestData data;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, clientindex);
	bsData->Read(data);
	//name = (char *)calloc(namelength + 1, sizeof(char));
	//bsData->Read(name, namelength);
	free(bsData);
	//name[namelength] = '\0';
	PrintToServer("Player %s[%d] connected", data.name, clientindex);
	if (!vmm.OnPlayerConnect(clientindex, data.name))
	{
		this->SendConnectionError(rpcParameters->sender, NetworkPlayerConnectionErrorScriptLock);
		net->CloseConnection(rpcParameters->sender, true);
		free(addressbuffer[clientindex]);
		addressbuffer[clientindex] = NULL;
		return;
	}
	if (!playm.RegisterNewPlayer(clientindex, data.name))
	{
		this->SendConnectionError(rpcParameters->sender, NetworkPlayerConnectionErrorAllocationError);
		net->CloseConnection(rpcParameters->sender, true);
		free(addressbuffer[clientindex]);
		addressbuffer[clientindex] = NULL;
		return;
	}
	//TODO: Recieve all server data (other players, vehicles...) and then send new player data to all. More fair.
	NetworkPlayerFullUpdateData *playerdata = this->GetPlayerFullUpdateData(clientindex);
	if (playerdata == NULL)
	{
		this->SendConnectionError(rpcParameters->sender, NetworkPlayerConnectionErrorAllocationError);
		net->CloseConnection(rpcParameters->sender, true);
		free(addressbuffer[clientindex]);
		addressbuffer[clientindex] = NULL;
		return;
	}
	RakNet::BitStream *bsSend = new RakNet::BitStream;
	bsSend->Write(playerdata);
	free(playerdata);
	this->SendDataToAll("ConnectPlayer", bsSend);
	free(bsSend);
	//TODO: Optimize using currently connected players, not buffer size.
	for (short i = 0; i < addressbuffersize; i++)
	{
		if (i != clientindex)
		{
			playerdata = this->GetPlayerFullUpdateData(i);
			if (playerdata != NULL)
			{
				bsSend = new RakNet::BitStream;
				bsSend->Write(playerdata);
				free(playerdata);
				net->RPC("ConnectPlayer", bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[clientindex][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
				free(bsSend);
			}
		}
	}
	NetworkVehicleFullUpdateData *vehicledata;
	for (short i = 0; i < vm.vehiclebuffersize; i++)
	{
		vehicledata = this->GetVehicleFullUpdateData(i);
		if (vehicledata != NULL)
		{
			bsSend = new RakNet::BitStream;
			bsSend->Write(vehicledata);
			free(vehicledata);
			net->RPC("CreateVehicle", bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[clientindex][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
			free(bsSend);
		}
	}
}

void NetworkManager::RecievePlayerMove(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	float position[3], angle, speed;
	bsData->Read(position[0]);
	bsData->Read(position[1]);
	bsData->Read(position[2]);
	bsData->Read(angle);
	bsData->Read(speed);
	free(bsData);
	if (!playm.MovePlayer(client, position, angle, speed))
	{
		return;
	}
	if (playm.playerbuffer[client]->vehicleindex != -1)
	{

		vm.SetVehiclePosition(playm.playerbuffer[client]->vehicleindex, position);
		vm.SetVehicleAngle(playm.playerbuffer[client]->vehicleindex, angle);
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(position[0]);
	bsSend.Write(position[1]);
	bsSend.Write(position[2]);
	bsSend.Write(angle);
	bsSend.Write(speed);
	this->SendDataToAllExceptOne("MovePlayer", client, &bsSend);
}

void NetworkManager::RecievePlayerJump(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	this->SendDataToAllExceptOne("JumpPlayer", client, &bsSend);
}

void NetworkManager::RecievePlayerDuck(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	bool shouldduck;
	bsData->Read(shouldduck);
	free(bsData);
	if (!playm.DuckPlayer(client, shouldduck))
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(shouldduck);
	this->SendDataToAllExceptOne("DuckPlayer", client, &bsSend);
}

void NetworkManager::RecievePlayerEnterInVehicle(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	short vehicleindex;
	int seat;
	bsData->Read(vehicleindex);
	bsData->Read(seat);
	free(bsData);
	if (!playm.PlayerEnterInVehicle(client, vehicleindex, seat))
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(vehicleindex);
	bsSend.Write(seat);
	this->SendDataToAllExceptOne("PlayerEnterInVehicle", client, &bsSend);
}

void NetworkManager::RecievePlayerCancelEnterInVehicle(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	free(bsData);
	if (!playm.PlayerExitFromVehicle(client))
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	this->SendDataToAllExceptOne("PlayerCancelEnterInVehicle", client, &bsSend);
}

void NetworkManager::RecievePlayerExitFromVehicle(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	free(bsData);
	if (!playm.PlayerExitFromVehicle(client))
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	this->SendDataToAllExceptOne("PlayerExitFromVehicle", client, &bsSend);
}

void NetworkManager::RecievePlayerFire(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	float x, y, z;
	int weapon, time;
	unsigned char index;
	unsigned int health, armour;
	bsData->Read(x);
	bsData->Read(y);
	bsData->Read(z);
	bsData->Read(weapon);
	bsData->Read(time);
	bsData->Read(index);
	bsData->Read(health);
	bsData->Read(armour);
	free(bsData);
	if (!playm.FirePlayer(client, weapon, index, health, armour))
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(index);
	bsSend.Write(x);
	bsSend.Write(y);
	bsSend.Write(z);
	bsSend.Write(weapon);
	bsSend.Write(time);
	this->SendDataToAllExceptOne("PlayerFire", client, &bsSend);
}

void NetworkManager::RecievePlayerAim(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	float x, y, z;
	int weapon, time;
	bsData->Read(x);
	bsData->Read(y);
	bsData->Read(z);
	bsData->Read(weapon);
	bsData->Read(time);
	free(bsData);
	if (!playm.ChangePlayerWeapon(client, weapon))
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(x);
	bsSend.Write(y);
	bsSend.Write(z);
	bsSend.Write(weapon);
	bsSend.Write(time);
	this->SendDataToAllExceptOne("PlayerAim", client, &bsSend);
}

void NetworkManager::RecievePlayerWeaponChange(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	int weapon;
	bsData->Read(weapon);
	free(bsData);
	if (!playm.ChangePlayerWeapon(client, weapon))
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(weapon);
	this->SendDataToAllExceptOne("SwapGun", client, &bsSend);
}

void NetworkManager::RecievePlayerHealthAndArmourChange(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	int health, armour;
	bsData->Read(health);
	bsData->Read(armour);
	if (!playm.SetPlayerHealth(client, health))
	{
		return;
	}
	if (!playm.SetPlayerArmour(client, armour))
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(health);
	bsSend.Write(armour);
	this->SendDataToAllExceptOne("PlayerParams", client, &bsSend);
}

void NetworkManager::RecievePlayerSpawnRequest(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	unsigned char playerclassindex;
	bsData->Read(playerclassindex);
	free(bsData);
	//SpawnInfo *spawn = playm.GetPlayerSpawnInfo(client, playerclassindex);
	//vmm.OnPlayerSpawn(client, playerclassindex);
	//RakNet::BitStream bsSend;
	//bsSend.Write(client);
	//bsSend.Write(spawn);
	//free(spawn);
	////TODO: Optimize using currently connected players, not buffer size.
	//for (unsigned char i = 0; i < addressbuffersize; i++)
	//{
	//	if (addressbuffer[i] != NULL)
	//	{
	//		net->RPC("PlayerSpawn", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[i][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
	//	}
	//}
}

void NetworkManager::RecievePlayerModelChange(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	int model;
	bsData->Read(model);
	free(bsData);
	if (!playm.SetPlayerModel(client, model))
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(model);
	this->SendDataToAllExceptOne("SyncSkin", client, &bsSend);
}

void NetworkManager::RecievePlayerComponentsChange(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	int compD[11], compT[11];
	bsData->Read(compD);
	bsData->Read(compT);
	free(bsData);
	if (!playm.SetPlayerComponents(client, compD, compT))
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(compD);
	bsSend.Write(compT);
	this->SendDataToAllExceptOne("SyncSkinVariation", client, &bsSend);
}

void NetworkManager::RecievePlayerChat(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	int len;
	char msg[128];
	bsData->Read(len);
	bsData->Read(msg);
	unsigned char color[4];
	if (!playm.GetPlayerColor(client, color))
	{
		return;
	}
	this->SendChatMessageToAll(client, msg, color);
}

bool NetworkManager::SendNewVehicleInfoToAll(const short index)
{
	if ((index < 0) || (index >= vm.vehiclebuffersize))
	{
		return false;
	}
	if (vm.vehiclebuffer[index] == NULL)
	{
		return false;
	}
	NetworkVehicleFullUpdateData data;
	data.index = index;
	data.model = vm.vehiclebuffer[index]->model;
	data.position[0] = vm.vehiclebuffer[index]->position[0];
	data.position[1] = vm.vehiclebuffer[index]->position[1];
	data.position[2] = vm.vehiclebuffer[index]->position[2];
	data.angle = vm.vehiclebuffer[index]->angle;
	data.color[0] = vm.vehiclebuffer[index]->color[0];
	data.color[1] = vm.vehiclebuffer[index]->color[2];
	RakNet::BitStream bsSend;
	bsSend.Write(data);
	this->SendDataToAll("CreateVehicle", &bsSend);
	return true;
}

short NetworkManager::RegisterNewClient(const SystemAddress address)
{
	short index = this->GetAddressFreeSlot();
	if (index == INVALID_PLAYER_INDEX)
	{
		return INVALID_PLAYER_INDEX;
	}
	if (index >= addressbuffersize)
	{
		if (addressbuffersize == maxaddressbuffersize)
		{
			return INVALID_PLAYER_INDEX;
		}
		if (!this->ResizeAddressBuffer(addressbuffer, index + 1))
		{
			return INVALID_PLAYER_INDEX;
		}
		addressbuffer[index] = NULL;
		addressbuffersize = index + 1;
	}
	addressbuffer[index] = new SystemAddress;
	addressbuffer[index][0] = address;
	return index;
}

short NetworkManager::GetClientIndex(const SystemAddress address)
{
	//TODO: Optimize using currently connected players, not buffer size.
	for (short index = 0; index < addressbuffersize; index++)
	{
		if ((addressbuffer[index] != NULL) && (addressbuffer[index][0] == address))
		{
			return index;
		}
	}
	return INVALID_PLAYER_INDEX;
}

short NetworkManager::GetAddressFreeSlot(void)
{
	short index;
	for (index = 0; index < addressbuffersize; index++)
	{
		if (addressbuffer[index] == NULL)
		{
			return index;
		}
	}
	if (addressbuffersize == maxaddressbuffersize)
	{
		return INVALID_PLAYER_INDEX;
	}
	return index;
}

bool NetworkManager::ResizeAddressBuffer(SystemAddress **&buffer, const short size)
{
	SystemAddress **tempbuffer = (SystemAddress **)realloc(*&buffer, size * sizeof(SystemAddress *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

RakNet::BitStream *NetworkManager::TranslateMessage(const RPCParameters *rpcParameters, short &index)
{
	if (rpcParameters == NULL)
	{
		return NULL;
	}
	index = this->GetClientIndex(rpcParameters->sender);
	if (index == INVALID_PLAYER_INDEX)
	{
		return NULL;
	}
	RakNet::BitStream *bs = new RakNet::BitStream(rpcParameters->input,((rpcParameters->numberOfBitsOfData)/8)+1, false);
	return bs;
}

void NetworkManager::SendConnectionError(const SystemAddress address, const NetworkPlayerConnectionError error)
{
	RakNet::BitStream bsSend;
	bsSend.Write(error);
	net->RPC("ErrorConnect", &bsSend, HIGH_PRIORITY,RELIABLE, 0, address, false, 0, UNASSIGNED_NETWORK_ID, 0);
}

NetworkPlayerFullUpdateData *NetworkManager::GetPlayerFullUpdateData(const short index)
{
	if ((index < 0) || (index >= addressbuffersize))
	{
		return NULL;
	}
	if (addressbuffer[index] == NULL)
	{
		return NULL;
	}
	if ((index < 0) || (index >= playm.playerbuffersize))
	{
		return NULL;
	}
	if (playm.playerbuffer[index] == NULL)
	{
		return NULL;
	}
	NetworkPlayerFullUpdateData *data = new NetworkPlayerFullUpdateData;
	data->index = index;
	strcpy(data->name, playm.playerbuffer[index]->name);
	data->model = playm.playerbuffer[index]->model;
	memcpy(data->position, playm.playerbuffer[index]->position, sizeof(float) * 3);
	data->angle = playm.playerbuffer[index]->angle;
	data->vehicleindex = playm.playerbuffer[index]->vehicleindex;
	data->seat_id = playm.playerbuffer[index]->seat_id;
	data->score = playm.playerbuffer[index]->score;
	data->health = playm.playerbuffer[index]->health;
	data->armour = playm.playerbuffer[index]->armour;
	data->room = playm.playerbuffer[index]->room;
	memcpy(data->weapons, playm.playerbuffer[index]->weapons, sizeof(int) * 8);
	memcpy(data->ammo, playm.playerbuffer[index]->ammo, sizeof(int) * 8);
	memcpy(data->color, playm.playerbuffer[index]->color, sizeof(unsigned char) * 4);
	return data;
}

NetworkVehicleFullUpdateData *NetworkManager::GetVehicleFullUpdateData(const short index)
{
	if ((index < 0) || (index >= vm.vehiclebuffersize))
	{
		return NULL;
	}
	if (vm.vehiclebuffer[index] == NULL)
	{
		return NULL;
	}
	NetworkVehicleFullUpdateData *data = new NetworkVehicleFullUpdateData;
	data->index = index;
	data->model = vm.vehiclebuffer[index]->model;
	memcpy(data->position, vm.vehiclebuffer[index]->position, sizeof(float) * 3);
	data->angle = vm.vehiclebuffer[index]->angle;
	memcpy(data->color, vm.vehiclebuffer[index]->color, sizeof(unsigned char) * 2);
	return data;
}

void NetworkManager::SendDataToAll(const char *RPC, const RakNet::BitStream *bsSend)
{
	//TODO: Optimize using currently connected players, not buffer size.
	for (short i = 0; i < addressbuffersize; i++)
	{
		if (addressbuffer[i] != NULL)
		{
			net->RPC(RPC, bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[i][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void NetworkManager::SendDataToAllExceptOne(const char *RPC, const short index, const RakNet::BitStream *bsSend)
{
	//TODO: Optimize using currently connected players, not buffer size.
	for (unsigned char i = 0; i < addressbuffersize; i++)
	{
		if ((i != index) && (addressbuffer[i] != NULL))
		{
			net->RPC(RPC, bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[i][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void NetworkManager::SendClassInfo(const short client)
{
	if ((client < 0) || (client >= addressbuffersize))
	{
		return;
	}
	if (addressbuffer[client] == NULL)
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(server.GetComponentSelectStatus());
	unsigned char numclasses = playm.GetNumberOfPlayerClasses();
	bsSend.Write(numclasses);
	int model;
	float position[3];
	float angle;
	int weapons[8];
	int ammo[8];
	unsigned char i = 0;
	unsigned char j = 0;
	while (j < numclasses)
	{
		if (playm.GetPlayerClassData(i, model, position, angle, weapons, ammo))
		{
			bsSend.Write(model);
			bsSend.Write(position);
			bsSend.Write(angle);
			bsSend.Write(weapons);
			bsSend.Write(ammo);
			j++;
		}
		i++;
	}
	net->RPC("ClassSync", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[client][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
}

void NetworkManager::SendChatMessageToAll(const short client, const char *message, const unsigned char color[4])
{
	if ((client < 0) || (client >= addressbuffersize))
	{
		return;
	}
	if (message == NULL)
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(strlen(message));
	bsSend.Write(message);
	bsSend.Write(color[0]);
	bsSend.Write(color[1]);
	bsSend.Write(color[2]);
	for (unsigned char i = 0; i < addressbuffersize; i++)
	{
		if (addressbuffer[i] != NULL)
		{
			net->RPC("Chat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[i][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}