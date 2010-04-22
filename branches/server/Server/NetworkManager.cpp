#include "net\RakNetworkFactory.h"
#include "net\MessageIdentifiers.h"
#include "net\BitStream.h"

#include "NetworkManager.h"
#include "logging.h"
#include "rpc.h"
#include "ServerCore.h"
#include "VirtualMachineManager.h"
#include "PlayerManager.h"

extern ServerCore server;
extern VirtualMachineManager vmm;
extern PlayerManager playm;

NetworkManager::NetworkManager(void)
{
	maxaddressbuffersize = playm.GetMaxPlayers();
	addressbuffersize = 0;
}

NetworkManager::~NetworkManager(void)
{
}

void NetworkManager::Init(unsigned char maxclients, unsigned short port)
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
					unsigned char clientindex;
					if (this->GetClientIndex(pack->systemAddress, clientindex));
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
	for (unsigned char i = 0; i < addressbuffersize; i++)
	{
		if (addressbuffer[i] != NULL)
		{
			net->RPC("Check", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[i][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
			debug("Check %d", i);
		}
	}
}

void NetworkManager::RemoveClient(const unsigned char index)
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
}

void NetworkManager::SendNewPlayerInfoToAll(const unsigned char index, const char *name, const int model, const float position[3], const float angle, const short vehicleindex, const int seat_id, const int score, const int health, const int armour, const int room, const int weapons[8], const int ammo[8], const unsigned char color[4])
{
	if (index >= addressbuffersize)
	{
		return;
	}
	if (name == NULL)
	{
		return;
	}
	// TODO: Optimize so it will also check for number of active connections.
	RakNet::BitStream bsSend;
	bsSend.Write(strlen(name));
	bsSend.Write(name, strlen(name));
	bsSend.Write(index);
	bsSend.Write(model);
	bsSend.Write(position[0]);
	bsSend.Write(position[1]);
	bsSend.Write(position[2]);
	bsSend.Write(angle);
	bsSend.Write(vehicleindex);
	bsSend.Write(seat_id);
	bsSend.Write(score);
	bsSend.Write(health);
	bsSend.Write(armour);
	bsSend.Write(room);
	bsSend.Write(weapons[0]);
	bsSend.Write(ammo[0]);
	bsSend.Write(weapons[1]);
	bsSend.Write(ammo[1]);
	bsSend.Write(weapons[2]);
	bsSend.Write(ammo[2]);
	bsSend.Write(weapons[3]);
	bsSend.Write(ammo[3]);
	bsSend.Write(weapons[4]);
	bsSend.Write(ammo[4]);
	bsSend.Write(weapons[5]);
	bsSend.Write(ammo[5]);
	bsSend.Write(weapons[6]);
	bsSend.Write(ammo[6]);
	bsSend.Write(weapons[7]);
	bsSend.Write(ammo[7]);
	bsSend.Write(color);
	for (unsigned char i = 0; i < addressbuffersize; i++)
	{
		if (addressbuffer[i] != NULL)
		{
			net->RPC("ConnectPlayer", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[index][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void NetworkManager::SendNewPlayerInfo(const unsigned char client, const unsigned char index, const char *name, const int model, const float position[3], const float angle, const short vehicleindex, const int seat_id, const int score, const int health, const int armour, const int room, const int weapons[8], const int ammo[8], const unsigned char color[4])
{
	if (client >= addressbuffersize)
	{
		return;
	}
	if (addressbuffer[client] == NULL)
	{
		return;
	}
	if (name == NULL)
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(strlen(name));
	bsSend.Write(name, strlen(name));
	bsSend.Write(index);
	bsSend.Write(model);
	bsSend.Write(position[0]);
	bsSend.Write(position[1]);
	bsSend.Write(position[2]);
	bsSend.Write(angle);
	bsSend.Write(vehicleindex);
	bsSend.Write(seat_id);
	bsSend.Write(score);
	bsSend.Write(health);
	bsSend.Write(armour);
	bsSend.Write(room);
	bsSend.Write(weapons[0]);
	bsSend.Write(ammo[0]);
	bsSend.Write(weapons[1]);
	bsSend.Write(ammo[1]);
	bsSend.Write(weapons[2]);
	bsSend.Write(ammo[2]);
	bsSend.Write(weapons[3]);
	bsSend.Write(ammo[3]);
	bsSend.Write(weapons[4]);
	bsSend.Write(ammo[4]);
	bsSend.Write(weapons[5]);
	bsSend.Write(ammo[5]);
	bsSend.Write(weapons[6]);
	bsSend.Write(ammo[6]);
	bsSend.Write(weapons[7]);
	bsSend.Write(ammo[7]);
	bsSend.Write(color);
	net->RPC("ConnectPlayer", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[client][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
}

void NetworkManager::DisconnectClient(const unsigned char index)
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
		this->SendConnectionError(rpcParameters->sender, ConnectionErrorServerFull);
		return;
	}
	unsigned char clientindex;
	if (!this->RegisterNewClient(rpcParameters->sender, clientindex))
	{
		this->SendConnectionError(rpcParameters->sender, ConnectionErrorAllocationError);
		return;
	}
	char *name;
	unsigned char namesize = 0;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, clientindex);
	bsData->Read(namesize);
	name = (char *)calloc(namesize + 1, sizeof(char));
	bsData->Read(name, namesize);
	free(bsData);
	name[namesize] = '\0';
	PrintToServer("Player %s[%d] connected", name, clientindex);
	playm.RegisterNewPlayer(clientindex, name);
}

void NetworkManager::RecievePlayerMove(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(position[0]);
	bsSend.Write(position[1]);
	bsSend.Write(position[2]);
	bsSend.Write(angle);
	bsSend.Write(speed);
	this->SendUpdateToAll("MovePlayer", client, &bsSend);
}

void NetworkManager::RecievePlayerJump(const RPCParameters *rpcParameters)
{
	unsigned char client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	this->SendUpdateToAll("JumpPlayer", client, &bsSend);
}

void NetworkManager::RecievePlayerDuck(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
	this->SendUpdateToAll("DuckPlayer", client, &bsSend);
}

void NetworkManager::RecievePlayerEnterInVehicle(const RPCParameters *rpcParameters)
{
	unsigned char client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	unsigned char vehicleindex;
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
	this->SendUpdateToAll("PlayerEnterInVehicle", client, &bsSend);
}

void NetworkManager::RecievePlayerCancelEnterInVehicle(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
	this->SendUpdateToAll("PlayerCancelEnterInVehicle", client, &bsSend);
}

void NetworkManager::RecievePlayerExitFromVehicle(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
	this->SendUpdateToAll("PlayerExitFromVehicle", client, &bsSend);
}

void NetworkManager::RecievePlayerFire(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
	this->SendUpdateToAll("PlayerFire", client, &bsSend);
}

void NetworkManager::RecievePlayerAim(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
	this->SendUpdateToAll("PlayerAim", client, &bsSend);
}

void NetworkManager::RecievePlayerWeaponChange(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
	this->SendUpdateToAll("SwapGun", client, &bsSend);
}

void NetworkManager::RecievePlayerHealthAndArmourChange(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
	this->SendUpdateToAll("PlayerParams", client, &bsSend);
}

void NetworkManager::RecievePlayerSpawnRequest(const RPCParameters *rpcParameters)
{
	unsigned char client;
	RakNet::BitStream *bsData = this->TranslateMessage(rpcParameters, client);
	if (bsData == NULL)
	{
		return;
	}
	unsigned char playerclassindex;
	bsData->Read(playerclassindex);
	free(bsData);
	SpawnInfo *spawn = playm.GetPlayerSpawnInfo(client, playerclassindex);
	vmm.OnPlayerSpawn(client, playerclassindex);
	RakNet::BitStream bsSend;
	bsSend.Write(client);
	bsSend.Write(spawn);
	free(spawn);
	//TODO: Optimize using currently connected players, not buffer size.
	for (unsigned char i = 0; i < addressbuffersize; i++)
	{
		if (addressbuffer[i] != NULL)
		{
			net->RPC("PlayerSpawn", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[i][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

void NetworkManager::RecievePlayerModelChange(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
	this->SendUpdateToAll("SyncSkin", client, &bsSend);
}

void NetworkManager::RecievePlayerComponentsChange(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
	this->SendUpdateToAll("SyncSkinVariation", client, &bsSend);
}

void NetworkManager::RecievePlayerChat(const RPCParameters *rpcParameters)
{
	unsigned char client;
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
void NetworkManager::SendConnectionError(const unsigned char index, const ConnectionError error)
{
	if (index >= addressbuffersize)
	{
		return;
	}
	if (addressbuffer[index] == NULL)
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(error);
	net->RPC("ErrorConnect", &bsSend, HIGH_PRIORITY,RELIABLE, 0, addressbuffer[index][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
}

void NetworkManager::SendVehicleInfo(const unsigned char client, const unsigned char index, const int model, const float position[3], const float angle, const int color[2])
{
	if (client >= addressbuffersize)
	{
		return;
	}
	if (addressbuffer[client] == NULL)
	{
		return;
	}
	RakNet::BitStream bsSend;
	bsSend.Write(index);
	bsSend.Write(model);
	bsSend.Write(position[0]);
	bsSend.Write(position[1]);
	bsSend.Write(position[2]);
	bsSend.Write(angle);
	bsSend.Write(color[0]);
	bsSend.Write(color[1]);
	net->RPC("CreateVehicle", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[client][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
}

void NetworkManager::SendClassInfo(const unsigned char client)
{
	if (client >= addressbuffersize)
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

void NetworkManager::SendNewVehicleToAll(const unsigned char index, const int model, const float position[3], const float angle, const int color[2])
{
	RakNet::BitStream bsSend;
	bsSend.Write(index);
	bsSend.Write(model);
	bsSend.Write(position[0]);
	bsSend.Write(position[1]);
	bsSend.Write(position[2]);
	bsSend.Write(angle);
	bsSend.Write(color[0]);
	bsSend.Write(color[1]);
	for(unsigned char i = 0; i < addressbuffersize; i++)
	{
		if(addressbuffer[i] != NULL)
		{
			net->RPC("CreateVehicle", &bsSend, HIGH_PRIORITY, RELIABLE, 0, addressbuffer[i][0], false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}

bool NetworkManager::RegisterNewClient(const SystemAddress address, unsigned char &index)
{
	if (!this->GetAddressFreeSlot(index))
	{
		return false;
	}
	if (index >= addressbuffersize)
	{
		if (addressbuffersize == maxaddressbuffersize)
		{
			return false;
		}
		if (!this->ResizeAddressBuffer(addressbuffer, index + 1))
		{
			return false;
		}
		addressbuffer[index] = NULL;
		addressbuffersize = index + 1;
	}
	addressbuffer[index] = new SystemAddress;
	addressbuffer[index][0] = address;
	return true;
}

bool NetworkManager::GetClientIndex(const SystemAddress address, unsigned char &index)
{
	for (index = 0; index < addressbuffersize; index++)
	{
		if ((addressbuffer[index] != NULL) && (addressbuffer[index][0] == address))
		{
			return true;
		}
	}
	return false;
}

bool NetworkManager::GetAddressFreeSlot(unsigned char &index)
{
	for (index = 0; index < addressbuffersize; index++)
	{
		if (addressbuffer[index] == NULL)
		{
			return true;
		}
	}
	if (addressbuffersize == maxaddressbuffersize)
	{
		return false;
	}
	return true;
}

bool NetworkManager::ResizeAddressBuffer(SystemAddress **&buffer, const unsigned char size)
{
	SystemAddress **tempbuffer = (SystemAddress **)realloc(*&buffer, size * sizeof(SystemAddress *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

RakNet::BitStream *NetworkManager::TranslateMessage(const RPCParameters *rpcParameters, unsigned char &index)
{
	if (rpcParameters == NULL)
	{
		return NULL;
	}
	if (!this->GetClientIndex(rpcParameters->sender, index))
	{
		return NULL;
	}
	RakNet::BitStream *bs = new RakNet::BitStream(rpcParameters->input,((rpcParameters->numberOfBitsOfData)/8)+1, false);
	return bs;
}

void NetworkManager::SendConnectionError(const SystemAddress address, const ConnectionError error)
{
	RakNet::BitStream bsSend;
	bsSend.Write(error);
	net->RPC("ErrorConnect", &bsSend, HIGH_PRIORITY,RELIABLE, 0, address, false, 0, UNASSIGNED_NETWORK_ID, 0);
}

void NetworkManager::SendUpdateToAll(const char *RPC, const unsigned char index, const RakNet::BitStream *bsSend)
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

void NetworkManager::SendChatMessageToAll(const unsigned char client, const char *message, const unsigned char color[4])
{
	if (client >= addressbuffersize)
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