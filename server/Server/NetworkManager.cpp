#include "RakNetworkFactory.h"
#include "MessageIdentifiers.h"

#include "NetworkManager.h"
#include "../../Shared/Console/common.h"
#include "logging.h"
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
	manager = NULL;
	rpc3 = NULL;
	net = NULL;
	maxclientbuffersize = playm.GetMaxPlayers();
	clientbuffer = NULL;
	clientbuffersize = 0;
}

NetworkManager::~NetworkManager(void)
{
	if (net != NULL)
	{
		net->Shutdown(100,0);
		RakNetworkFactory::DestroyRakPeerInterface(net);
	}
	if (rpc3)
	{
		delete rpc3;
	}
	this->networkID = UNASSIGNED_NETWORK_ID;
	if (manager)
	{
		delete manager;
	}
	if (clientbuffer != NULL)
	{
		for (short i = 0; i < clientbuffersize; i++)
		{
			if (clientbuffer[i] != NULL)
			{
				delete clientbuffer[i];
			}
		}
		free(clientbuffer);
	}
}

void NetworkManager::Load(const short maxclients, const unsigned short port)
{
	manager = new NetworkIDManager;
	manager->SetIsNetworkIDAuthority(true);
	serverid.localSystemAddress = 0;
	defaultclientid.localSystemAddress = 65534;
	this->SetNetworkIDManager(manager);
	this->SetNetworkID(serverid);
	rpc3 = new RakNet::RPC3;
	rpc3->SetNetworkIDManager(manager);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecieveClientConnectionRequest);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerMove);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerJump);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerDuck);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerEntranceInVehicle);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerCancelEntranceInVehicle);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerExitFromVehicle);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerFire);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerAim);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerWeaponChange);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerHealthAndArmorChange);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerSpawnRequest);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerModelChange);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerComponentsChange);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerChat);
	net = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor s(port, 0);
	net->SetMaximumIncomingConnections(maxclients);
	net->Startup(maxclients, 1, &s, 1);
	net->AttachPlugin(rpc3);
}

void NetworkManager::Tick(void)
{
	static Packet *pack;
	for (pack = net->Receive(); pack; net->DeallocatePacket(pack), pack = net->Receive())
	{
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
					delete playm.playerbuffer[client];
					playm.playerbuffer[client] = NULL;
					playm.numplayers--;
					if ((client < 0) && (client >= clientbuffersize))
					{
						break;
					}
					if (clientbuffer[client] == NULL)
					{
						break;
					}
					delete clientbuffer[client];
					clientbuffer[client] = NULL;
					NetworkPlayerDisconnectionData data;
					data.client = client;
					this->SendDataToAll("&NetworkManager::RecieveClientDisconnection", &data);
					this->UpdateServerInfo();
				}
				break;
			}
		}
	}
}

void NetworkManager::Unload(void)
{
	if (net != NULL)
	{
		net->Shutdown(100,0);
		RakNetworkFactory::DestroyRakPeerInterface(net);
		net = NULL;
	}
	if (rpc3)
	{
		delete rpc3;
		rpc3 = NULL;
	}
	if (manager)
	{
		delete manager;
		manager = NULL;
	}
	if (clientbuffer != NULL)
	{
		for (short i = 0; i < clientbuffersize; i++)
		{
			if (clientbuffer[i] != NULL)
			{
				delete clientbuffer[i];
				clientbuffer[i] = NULL;
			}
		}
		free(clientbuffer);
		clientbuffer = NULL;
	}
}

void NetworkManager::CheckClients(void)
{
	//this->SendDataToAll("&NetworkManager::RecieveCheck", );
}

void NetworkManager::UpdateServerInfo(void)
{
	char *hostname = server.GetHostname();
	char *gamemode = vmm.GetGameModeName();
	char *info = new char[512];
	sprintf(info, "%s%c%s%c%s%c%d%c%d%c%d%c%s%c", hostname, 1, gamemode, 1, "World", 1, playm.numplayers, 1, playm.maxplayerbuffersize, 1, server.IsPasswordProtected(), 1, "", 1);
	net->SetOfflinePingResponse(info, strlen(info));
	free(hostname);
	free(gamemode);
	delete info;
}

void NetworkManager::RecieveClientConnectionRequest(NetworkPlayerConnectionRequestData data, RakNet::RPC3 *clientrpc3)
{
	if (playm.IsServerFull())
	{
		this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorServerFull);
		net->CloseConnection(clientrpc3->GetLastSenderAddress(), true);
		return;
	}
	if (data.protocol != PROTOCOL_VERSION)
	{
		this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorServerFull);
		net->CloseConnection(clientrpc3->GetLastSenderAddress(), true);
	}
	short clientindex = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (clientindex != INVALID_PLAYER_INDEX)
	{
		PrintToServer("Player %s has tried to connect twice.", playm.GetPlayerName(clientindex));
		this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorAlreadyConnected);
	}
	clientindex = this->RegisterNewClient(clientrpc3->GetLastSenderAddress());
	if (clientindex == INVALID_PLAYER_INDEX)
	{
		PrintToServer("Unable to register new client.");
		this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorAllocationError);
		net->CloseConnection(clientrpc3->GetLastSenderAddress(), true);
		return;
	}
	PrintToServer("Player %s[%d] connected", data.name, clientindex);
	if (!vmm.OnPlayerConnect(clientindex, data.name))
	{
		this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorScriptLock);
		net->CloseConnection(clientrpc3->GetLastSenderAddress(), true);
		delete clientbuffer[clientindex];
		clientbuffer[clientindex] = NULL;
		return;
	}
	if (!playm.RegisterNewPlayer(clientindex, data.name))
	{
		this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorAllocationError);
		net->CloseConnection(clientrpc3->GetLastSenderAddress(), true);
		delete clientbuffer[clientindex];
		clientbuffer[clientindex] = NULL;
		return;
	}
	//TODO: Recieve all server data (other players, vehicles...) and then send new player data to all. More fair.
	NetworkPlayerFullUpdateData *playerdata;
	//TODO: Optimize using currently connected players, not buffer size.
	for (short i = 0; i < clientbuffersize; i++)
	{
		if (i != clientindex)
		{
			playerdata = this->GetPlayerFullUpdateData(i);
			if (playerdata != NULL)
			{
				rpc3->CallCPP("&NetworkManager::RecievePlayerFullUpdate", defaultclientid, *playerdata);
				delete playerdata;
			}
		}
	}
	NetworkVehicleFullUpdateData *vehicledata;
	for (short i = 0; i < vm.vehiclebuffersize; i++)
	{
		vehicledata = this->GetVehicleFullUpdateData(i);
		if (vehicledata != NULL)
		{
			rpc3->CallCPP("&NetworkManager::RecieveVehicleFullUpdate", defaultclientid, *vehicledata);
			delete vehicledata;
		}
	}
	playerdata = this->GetPlayerFullUpdateData(clientindex);
	if (playerdata == NULL)
	{
		this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorAllocationError);
		net->CloseConnection(clientrpc3->GetLastSenderAddress(), true);
		delete clientbuffer[clientindex];
		clientbuffer[clientindex] = NULL;
		return;
	}
	rpc3->CallCPP("&NetworkManager::RecieveClientConnection", defaultclientid, *playerdata);
	this->SendDataToAllExceptOne("&NetworkManager::RecieveClientConnection", clientindex, playerdata);
	delete playerdata;
	this->UpdateServerInfo();
}

void NetworkManager::RecievePlayerMove(NetworkPlayerMoveData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerMove", client, &data);
}

void NetworkManager::RecievePlayerJump(NetworkPlayerJumpData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerJump", client, &data);
}

void NetworkManager::RecievePlayerDuck(NetworkPlayerDuckData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerDuck", client, &data);
}

void NetworkManager::RecievePlayerEntranceInVehicle(NetworkPlayerEntranceInVehicleData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerEntranceInVehicle", client, &data);
}

void NetworkManager::RecievePlayerCancelEntranceInVehicle(NetworkPlayerCancelEntranceInVehicleData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	data.client = client;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerCancelEntranceInVehicle", client, &data);
}

void NetworkManager::RecievePlayerExitFromVehicle(NetworkPlayerExitFromVehicleData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	data.client = client;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerExitFromVehicle", client, &data);
}

void NetworkManager::RecievePlayerFire(NetworkPlayerFireData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerFire", client, &data);
}

void NetworkManager::RecievePlayerAim(NetworkPlayerAimData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerAim", client, &data);
}

void NetworkManager::RecievePlayerWeaponChange(NetworkPlayerWeaponChangeData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerWeaponChange", client, &data);
}

void NetworkManager::RecievePlayerHealthAndArmorChange(NetworkPlayerHealthAndArmorChangeData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerHealthAndArmorChange", client, &data);
}

void NetworkManager::RecievePlayerSpawnRequest(NetworkPlayerSpawnRequestData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	this->SendDataToAll("&NetworkManager::RecievePlayerSpawn", &data);
}

void NetworkManager::RecievePlayerModelChange(NetworkPlayerModelChangeData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerModelChange", client, &data);
}

void NetworkManager::RecievePlayerComponentsChange(NetworkPlayerComponentsChangeData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
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
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerComponentsChange", client, &data);
}

void NetworkManager::RecievePlayerChat(NetworkPlayerChatData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	if (!playm.GetPlayerColor(client, data.color))
	{
		return;
	}
	if(!vmm.OnPlayerText(client, data.msg))
		return;

	data.client = client;
	this->SendDataToAll("&NetworkManager::RecievePlayerChat", &data);
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
	this->SendDataToAll("&NetworkManager::RecieveNewVehicle", &data);
	return true;
}

short NetworkManager::RegisterNewClient(const SystemAddress address)
{
	short index = this->GetClientFreeSlot();
	if (index == INVALID_PLAYER_INDEX)
	{
		return INVALID_PLAYER_INDEX;
	}
	if (index >= clientbuffersize)
	{
		if (clientbuffersize == maxclientbuffersize)
		{
			return INVALID_PLAYER_INDEX;
		}
		if (!ResizeBuffer<Client **, Client *, short>(clientbuffer, index + 1))
		{
			return INVALID_PLAYER_INDEX;
		}
		clientbuffer[index] = NULL;
		clientbuffersize = index + 1;
	}
	clientbuffer[index] = new Client;
	clientbuffer[index]->address = address;
	clientbuffer[index]->id.localSystemAddress = index + 1;
	return index;
}

short NetworkManager::GetClientIndex(const SystemAddress address)
{
	//TODO: Optimize using currently connected players, not buffer size.
	for (short index = 0; index < clientbuffersize; index++)
	{
		if ((clientbuffer[index] != NULL) && (clientbuffer[index]->address == address))
		{
			return index;
		}
	}
	return INVALID_PLAYER_INDEX;
}

short NetworkManager::GetClientFreeSlot(void)
{
	short index;
	for (index = 0; index < clientbuffersize; index++)
	{
		if (clientbuffer[index] == NULL)
		{
			return index;
		}
	}
	if (clientbuffersize == maxclientbuffersize)
	{
		return INVALID_PLAYER_INDEX;
	}
	return index;
}

void NetworkManager::SendConnectionError(const SystemAddress address, const NetworkPlayerConnectionError error)
{
	NetworkPlayerConnectionErrorData data;
	data.error = error;
	rpc3->CallCPP("&NetworkManager::RecieveClientConnectionError", defaultclientid, data, rpc3);
}

NetworkPlayerFullUpdateData *NetworkManager::GetPlayerFullUpdateData(const short index)
{
	if ((index < 0) || (index >= clientbuffersize))
	{
		return NULL;
	}
	if (clientbuffer[index] == NULL)
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
	memcpy(data->weapons, playm.playerbuffer[index]->weapons, sizeof(char) * 8);
	memcpy(data->ammo, playm.playerbuffer[index]->ammo, sizeof(short) * 8);
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

template <typename DATATYPE>
void NetworkManager::SendDataToAll(const char *RPC, const DATATYPE *data)
{
	//TODO: Optimize using currently connected players, not buffer size.
	for (short i = 0; i < clientbuffersize; i++)
	{
		if (clientbuffer[i] != NULL)
		{
			rpc3->CallCPP(RPC, clientbuffer[i]->id, *data, rpc3);
		}
	}
}

template <typename DATATYPE>
void NetworkManager::SendDataToAllExceptOne(const char *RPC, const short index, const DATATYPE *data)
{
	//TODO: Optimize using currently connected players, not buffer size.
	for (unsigned char i = 0; i < clientbuffersize; i++)
	{
		if ((i != index) && (clientbuffer[i] != NULL))
		{
			rpc3->CallCPP(RPC, clientbuffer[i]->id, *data, rpc3);
		}
	}
}

//void NetworkManager::SendClassInfo(const short client)
//{
//	if ((client < 0) || (client >= addressbuffersize))
//	{
//		return;
//	}
//	if (addressbuffer[client] == NULL)
//	{
//		return;
//	}
//	RakNet::BitStream bsSend;
//	bsSend.Write(server.GetComponentSelectStatus());
//	unsigned char numclasses = playm.GetNumberOfPlayerClasses();
//	bsSend.Write(numclasses);
//	unsigned int model;
//	float position[3];
//	float angle;
//	char weapons[8];
//	short ammo[8];
//	unsigned char i = 0;
//	unsigned char j = 0;
//	while (j < numclasses)
//	{
//		if (playm.GetPlayerClassData(i, model, position, angle, weapons, ammo))
//		{
//			bsSend.Write(model);
//			bsSend.Write(position);
//			bsSend.Write(angle);
//			bsSend.Write(weapons);
//			bsSend.Write(ammo);
//			j++;
//		}
//		i++;
//	}
//	net->RPC("ClassSync", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[client][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
//}