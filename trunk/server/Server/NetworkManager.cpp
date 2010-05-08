#include "../../Shared/RakNet/RakNetworkFactory.h"
#include "../../Shared/RakNet/MessageIdentifiers.h"
#include "../../Shared/RakNet/BitStream.h"

#include "NetworkManager.h"
#include "../../Shared/Console/common.h"
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
					short client = this->GetClientIndex(pack->systemAddress);
					if (client != INVALID_PLAYER_INDEX)
					{
						if ((client < 0) || (client >= playm.playerbuffersize))
						{
							break;
						}
						if (playm.playerbuffer[client] == NULL)
						{
							break;
						}
						vmm.OnPlayerDisconnect(client);
						PrintToServer("Player %s disconnected", playm.playerbuffer[client]->name);
						free(playm.playerbuffer[client]);
						playm.playerbuffer[client] = NULL;
						playm.numplayers--;
						if ((client < 0) && (client >= addressbuffersize))
						{
							break;
						}
						if (addressbuffer[client] == NULL)
						{
							break;
						}
						free(addressbuffer[client]);
						addressbuffer[client] = NULL;
						NetworkPlayerDisconnectionData data;
						data.client = client;
						RakNet::BitStream bsSend;
						bsSend.Write(data);
						this->SendDataToAll("Disconnect", &bsSend);
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

void NetworkManager::UpdateServerInfo(char *name, char *mode, char *loc, short players, short maxplayers, bool password, char *clan)
{
	char *info = new char[512];
	sprintf(info, "%s%c%s%c%s%c%d%c%d%c%d%c%s%c", name, 1, mode, 1, loc, 1, players, 1, maxplayers, 1, password, 1, clan, 1);
	net->SetOfflinePingResponse(info, strlen(info));
	free(info);
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
	NetworkPlayerConnectionRequestData data;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, clientindex);
	bsData->Read(data);
	free(bsData);
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
	bsSend->Write(*playerdata);
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
				bsSend->Write(*playerdata);
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
			bsSend->Write(*vehicledata);
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
	NetworkPlayerMoveData data;
	bsData->Read(data);
	free(bsData);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	memcpy(playm.playerbuffer[client]->position, data.position, sizeof(float) * 3);
	playm.playerbuffer[client]->angle = data.angle;
	if (playm.playerbuffer[client]->car_enter == true)
	{
		return;
	}
	if (playm.playerbuffer[client]->vehicleindex != -1)
	{

		vm.SetVehiclePositionInternal(playm.playerbuffer[client]->vehicleindex, data.position);
		vm.SetVehicleAngleInternal(playm.playerbuffer[client]->vehicleindex, data.angle);
	}
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
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
	NetworkPlayerJumpData data;
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
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
	NetworkPlayerDuckData data;
	bsData->Read(data);
	free(bsData);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	playm.playerbuffer[client]->isducking = data.shouldduck;
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
	this->SendDataToAllExceptOne("DuckPlayer", client, &bsSend);
}

void NetworkManager::RecievePlayerEntranceInVehicle(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	NetworkPlayerEntranceInVehicleData data;
	bsData->Read(data);
	free(bsData);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	if (data.seat == INVALID_PLAYER_SEAT_INDEX)
	{
		playm.playerbuffer[client]->car_enter = false;
		return;
	}
	playm.playerbuffer[client]->car_enter = true;
	playm.playerbuffer[client]->seatindex = data.seat;
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
	this->SendDataToAllExceptOne("PlayerEnterInVehicle", client, &bsSend);
}

void NetworkManager::RecievePlayerCancelEntranceInVehicle(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	free(bsData);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	playm.playerbuffer[client]->vehicleindex = -1;
	playm.playerbuffer[client]->car_enter = false;
	NetworkPlayerCancelEntranceInVehicleData data;
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
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
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	playm.playerbuffer[client]->vehicleindex = -1;
	playm.playerbuffer[client]->car_enter = false;
	NetworkPlayerExitFromVehicleData data;
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
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
	NetworkPlayerFireData data;
	bsData->Read(data);
	free(bsData);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	playm.playerbuffer[client]->currentweapon = data.weapon;
	if (data.target != -1)
	{
		if (data.target >= playm.playerbuffersize)
		{
			return;
		}
		if (playm.playerbuffer[data.target] == NULL)
		{
			return;
		}
		playm.playerbuffer[data.target]->health = data.health;
		playm.playerbuffer[data.target]->armor = data.armor;
	}
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
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
	NetworkPlayerAimData data;
	bsData->Read(data);
	free(bsData);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	playm.playerbuffer[client]->currentweapon = data.weapon;
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
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
	NetworkPlayerWeaponChangeData data;
	bsData->Read(data);
	free(bsData);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	playm.playerbuffer[client]->currentweapon = data.weapon;
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
	this->SendDataToAllExceptOne("SwapGun", client, &bsSend);
}

void NetworkManager::RecievePlayerHealthAndArmorChange(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	NetworkPlayerHealthAndArmorChangeData data;
	bsData->Read(data);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	playm.playerbuffer[client]->health = data.health;
	playm.playerbuffer[client]->armor = data.armor;
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
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
	NetworkPlayerSpawnRequestData data;
	bsData->Read(data);
	free(bsData);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	if (data.playerclassindex >= playm.classbuffersize)
	{
		return;
	}
	if (playm.classbuffer[data.playerclassindex] == NULL)
	{
		return;
	}
	NetworkPlayerSpawnData data2;
	data2.armor = 0;
	data2.health = 200;
	data2.model = playm.classbuffer[data.playerclassindex]->model;
	data2.angle = playm.classbuffer[data.playerclassindex]->angle;
	data2.room = 0;
	memcpy(data2.position, playm.classbuffer[data.playerclassindex]->position, sizeof(float) * 3);
	memcpy(data2.compD, playm.playerbuffer[client]->compD, sizeof(int) * 11);
	memcpy(data2.compT, playm.playerbuffer[client]->compT, sizeof(int) * 11);
	vmm.OnPlayerSpawn(client, data.playerclassindex);
	data2.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data2);
	this->SendDataToAll("PlayerSpawn", &bsSend);
}

void NetworkManager::RecievePlayerModelChange(const RPCParameters *rpcParameters)
{
	short client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	NetworkPlayerModelChangeData data;
	bsData->Read(data);
	free(bsData);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	playm.playerbuffer[client]->model = data.model;
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
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
	NetworkPlayerComponentsChangeData data;
	bsData->Read(data);
	free(bsData);
	if ((client < 0) || (client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[client] == NULL)
	{
		return;
	}
	memcpy(playm.playerbuffer[client]->compD, data.compD, sizeof(int) * 11);
	memcpy(playm.playerbuffer[client]->compT, data.compT, sizeof(int) * 11);
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
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
	NetworkPlayerChatData data;
	bsData->Read(data);
	free(bsData);
	if (!playm.GetPlayerColor(client, data.color))
	{
		return;
	}
	data.client = client;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
	this->SendDataToAllExceptOne("Chat", client, &bsSend);
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
	data.angle = vm.vehiclebuffer[index]->angle[1];
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
		if (!ResizeBuffer<SystemAddress **, SystemAddress *, short>(addressbuffer, index + 1))
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
	NetworkPlayerConnectionErrorData data;
	data.error = error;
	RakNet::BitStream bsSend;
	bsSend.Write(data);
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
	data->seatindex = playm.playerbuffer[index]->seatindex;
	data->score = playm.playerbuffer[index]->score;
	data->health = playm.playerbuffer[index]->health;
	data->armor = playm.playerbuffer[index]->armor;
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
	data->angle = vm.vehiclebuffer[index]->angle[1];
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