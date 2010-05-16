#include "RakNetworkFactory.h"
#include "MessageIdentifiers.h"
#include "GetTime.h"

#include "NetworkManager.h"
#include "main.h"
#include "log.h"
#include "ClientCore.h"
#include "Hook/classes.h"
#include "masterserver.h"
#include "chat.h"
#include "d3d9/gui.h"

extern ClientCore client;
extern FMPHook HOOK;
extern FMPGUI Gui;

extern ClientState clientstate;
extern unsigned char selectedplayerclass;
extern int LastUpdate;
extern char enterMsg[256];

NetworkManager::NetworkManager(void)
{
	manager = NULL;
	rpc3 = NULL;
	net = NULL;
}

NetworkManager::~NetworkManager(void)
{
	if (net)
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
}

void NetworkManager::Load(void)
{
	manager = new NetworkIDManager;
	clientid.localSystemAddress = DEFAULT_CLIENT_NETWORK_ID;;
	serverid.localSystemAddress = DEFAULT_SERVER_NETWORK_ID;;
	this->SetNetworkIDManager(manager);
	this->SetNetworkID(clientid);
	rpc3 = new RakNet::RPC3;
	rpc3->SetNetworkIDManager(manager);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecieveClientConnection);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecieveClientConnectionError);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecieveClientInfo);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecieveClientDisconnection);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerFullUpdate);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecieveVehicleFullUpdate);
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
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerSpawn);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerModelChange);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerComponentsChange);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerChat);
	net = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor socketDescriptor(0,0);
	net->Startup(1, 1, &socketDescriptor, 1);
	net->AttachPlugin(rpc3);
}

void NetworkManager::Tick(void)
{
	static Packet *pack;
	for (pack = net->Receive(); pack; net->DeallocatePacket(pack), pack = net->Receive())
	{
		Debug("Pack: %s[%d], %s", pack->data, pack->data[0], pack->systemAddress.ToString());
		switch(pack->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				Log("RakNet: Connection accepted. Sending client info...");
				this->SendClientConnectionRequest();
			} break;
		case ID_ALREADY_CONNECTED:
			{
				Log("RakNet: Already connected");
				clientstate.game = GameStateInGame;	
			} break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			{
				Log("RakNet: No free connections");
				clientstate.game = GameStateOffline;
			} break;
		case ID_DISCONNECTION_NOTIFICATION:
			{
				Log("RakNet: Disconnect (Close connection)");
				clientstate.game = GameStateOffline;
			} break;
		case ID_CONNECTION_LOST:
			{
				Log("RakNet: Disconnect (Connection lost)");
				clientstate.game = GameStateOffline;
			} break;
		case ID_CONNECTION_BANNED:
			{
				Log("RakNet: Disconnect (Connection banned)");
				clientstate.game = GameStateOffline;
			} break;
		case ID_INVALID_PASSWORD:
			{
				Log("RakNet: Invalid password");
			} break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			{
				Log("RakNet: Connection failed");
				clientstate.game = GameStateOffline;
				char str[128];
				sprintf(str, "Can't connect to %s", pack->systemAddress.ToString());
				Gui.Message(str);
			} break;
		case ID_PONG:
			{
				Log("RakNet: Pong");
				RakNetTime time, dataLength;
				RakNet::BitStream pong( pack->data+1, sizeof(RakNetTime), false);
				pong.Read(time);
				dataLength = pack->length - sizeof(unsigned char) - sizeof(RakNetTime);
				MasterServerInfo *tmp_msi = new MasterServerInfo;
				tmp_msi->ping = (unsigned int)(RakNet::GetTime()-time);
				Debug("ID_PONG from SystemAddress:%u:%u.", pack->systemAddress.binaryAddress, pack->systemAddress.port);
				Debug("Time is %i",time);
				Debug("Ping is %i", tmp_msi->ping);
				Debug("Data is %i bytes long.", dataLength);
				if (dataLength > 0)
				{
					Debug("Data is %s\n", pack->data+sizeof(unsigned char)+sizeof(RakNetTime));
					unsigned char *data = pack->data+sizeof(unsigned char)+sizeof(RakNetTime);
					sscanf((char*)data, "%[^\1]\1%[^\1]\1%[^\1]\1%d\1%d\1%d\1%[^\1]\1", &tmp_msi->name, &tmp_msi->mode, &tmp_msi->loc, 
					&tmp_msi->players, &tmp_msi->maxplayers, &tmp_msi->password, &tmp_msi->clan);
				}
				strcpy_s(tmp_msi->ip, 64, pack->systemAddress.ToString(0));
				tmp_msi->port = pack->systemAddress.port;
				Gui.UpdateServer(tmp_msi);
			} break;
		case ID_RPC_REMOTE_ERROR:
			{
				Log("RakNet: RPC remote error");
				switch (pack->data[1])
				{
				case RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE:
					Log("RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE\n");
					break;
				case RPC_ERROR_OBJECT_DOES_NOT_EXIST:
					Log("RPC_ERROR_OBJECT_DOES_NOT_EXIST\n");
					break;
				case RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE:
					Log("RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE\n");
					break;
				case RPC_ERROR_FUNCTION_NOT_REGISTERED:
					Log("RPC_ERROR_FUNCTION_NOT_REGISTERED\n");
					break;
				case RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED:
					Log("RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED\n");
					break;
				case RPC_ERROR_CALLING_CPP_AS_C:
					Log("RPC_ERROR_CALLING_CPP_AS_C\n");
					break;
				case RPC_ERROR_CALLING_C_AS_CPP:
					Log("RPC_ERROR_CALLING_C_AS_CPP\n");
					break;
				}
			} break;
		default:
			{
				Log("RakNet: Unknown message (0x%x) [%s]", pack->data[0], pack->data);
			} break;
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
}

void NetworkManager::Ping(const char *hostname, const unsigned short port)
{
	net->Ping(hostname, port, false);
}

void NetworkManager::ConnectToServer(const char *hostname, const unsigned short port)
{
	LastUpdate = GetTickCount();

	// Коннект
	Log("Connecting to server...");
	if(clientstate.game == GameStateOffline)
	{
		if(hostname[0] == 0 || port == 0)
		{
			net->Connect(Conf.server, Conf.port, 0, 0, 0);
			serveraddress.SetBinaryAddress(Conf.server);
			serveraddress.port = Conf.port;
		}
		else
		{
			net->Connect(hostname, port, 0, 0, 0);
			serveraddress.SetBinaryAddress(hostname);
			serveraddress.port = port;
		}
	}
	else if(clientstate.game == GameStateConnecting)
	{
		net->Connect(serveraddress.ToString(0), serveraddress.port, 0, 0, 0);
	}

	clientstate.game = GameStateConnecting;
}

void NetworkManager::SendClientConnectionRequest(void)
{
	NetworkPlayerConnectionRequestData data;
	data.protocol = PROTOCOL_VERSION;
	strcpy(data.name, Conf.Name);
	data.sessionkey = client.GetSessionKey();
	rpc3->CallCPP("&NetworkManager::RecieveClientConnectionRequest", serverid, data, rpc3);
}

void NetworkManager::SendPlayerMove(const float speed)
{
	NetworkPlayerMoveData data;
	memcpy(data.position, gPlayer[client.GetIndex()].position, sizeof(float) * 3);
	data.angle = gPlayer[client.GetIndex()].angle;
	data.speed = speed;
	rpc3->CallCPP("&NetworkManager::RecievePlayerMove", serverid, data, rpc3);
}

void NetworkManager::SendPlayerJump(void)
{
	NetworkPlayerJumpData data;
	rpc3->CallCPP("&NetworkManager::RecievePlayerJump", serverid, data, rpc3);
}

void NetworkManager::SendPlayerDuck(void)
{
	NetworkPlayerDuckData data;
	data.shouldduck = gPlayer[client.GetIndex()].isducking;
	rpc3->CallCPP("&NetworkManager::RecievePlayerDuck", serverid, data, rpc3);
}

void NetworkManager::SendPlayerEntranceInVehicle(const char seat)
{
	NetworkPlayerEntranceInVehicleData data;
	data.vehicleindex = gPlayer[client.GetIndex()].vehicleindex;
	data.seat = seat;
	rpc3->CallCPP("&NetworkManager::RecievePlayerEntranceInVehicle", serverid, data, rpc3);
}

void NetworkManager::SendPlayerCancelEntranceInVehicle(void)
{
	NetworkPlayerCancelEntranceInVehicleData data;
	rpc3->CallCPP("&NetworkManager::RecievePlayerCancelEntranceInVehicle", serverid, data, rpc3);
}

void NetworkManager::SendPlayerExitFromVehicle(void)
{
	NetworkPlayerExitFromVehicleData data;
	rpc3->CallCPP("&NetworkManager::RecievePlayerExitFromVehicle", serverid, data, rpc3);
}

void NetworkManager::SendPlayerFire(const float position[3], const int time, const short target, const unsigned int health, const unsigned int armor)
{
	NetworkPlayerFireData data;
	memcpy(data.position, position, sizeof(float) * 3);
	data.weapon = gPlayer[client.GetIndex()].currentweapon;
	data.time = time;
	data.target = target;
	data.health = health;
	data.armor = armor;
	rpc3->CallCPP("&NetworkManager::RecievePlayerFire", serverid, data, rpc3);
}

void NetworkManager::SendPlayerAim(const float position[3], const int time)
{
	NetworkPlayerAimData data;
	memcpy(data.position, position, sizeof(float) * 3);
	data.weapon = gPlayer[client.GetIndex()].currentweapon;
	data.time = time;
	rpc3->CallCPP("&NetworkManager::RecievePlayerAim", serverid, data, rpc3);
}

void NetworkManager::SendPlayerWeaponChange(void)
{
	NetworkPlayerWeaponChangeData data;
	data.weapon = gPlayer[client.GetIndex()].currentweapon;
	rpc3->CallCPP("&NetworkManager::RecievePlayerWeaponChange", serverid, data, rpc3);
}

void NetworkManager::SendPlayerHealthAndArmorChange(void)
{
	NetworkPlayerHealthAndArmorChangeData data;
	data.health = gPlayer[client.GetIndex()].health;
	data.armor = gPlayer[client.GetIndex()].armor;
	rpc3->CallCPP("&NetworkManager::RecievePlayerHealthAndArmorChange", serverid, data, rpc3);
}

void NetworkManager::SendPlayerSpawnRequest(void)
{
	NetworkPlayerSpawnRequestData data;
	data.playerclassindex = selectedplayerclass;
	rpc3->CallCPP("&NetworkManager::RecievePlayerSpawnRequest", serverid, data, rpc3);
	Log("Player spawn request has been sent.");
}

void NetworkManager::SendPlayerModelChange(void)
{
	NetworkPlayerModelChangeData data;
	data.model = gPlayer[client.GetIndex()].model;
	rpc3->CallCPP("&NetworkManager::RecievePlayerModelChange", serverid, data, rpc3);
	Log("Player model change has been sent.");
}

void NetworkManager::SendPlayerComponentsChange(void)
{
	NetworkPlayerComponentsChangeData data;
	memcpy(data.compD, gPlayer[client.GetIndex()].compD, sizeof(int) * 11);
	memcpy(data.compT, gPlayer[client.GetIndex()].compT, sizeof(int) * 11);
	rpc3->CallCPP("&NetworkManager::RecievePlayerComponentsChange", serverid, data, rpc3);
}

void NetworkManager::SendPlayerChat(void)
{
	NetworkPlayerChatData data;
	strcpy(data.msg, enterMsg);
	rpc3->CallCPP("&NetworkManager::RecievePlayerChat", serverid, data, rpc3);
}

void NetworkManager::RecieveClientConnection(NetworkPlayerFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
	Log("New player connection. Name is %s", data.name);
	LastUpdate = GetTickCount();
	gPlayer[data.index].model = data.model;
	memcpy(gPlayer[data.index].position, data.position, sizeof(float) * 3);
	gPlayer[data.index].angle = data.angle;
	gPlayer[data.index].vehicleindex = data.vehicleindex;
	gPlayer[data.index].seatindex = data.seatindex;
	gPlayer[data.index].score = data.score;
	gPlayer[data.index].health = data.health;
	gPlayer[data.index].armor = data.armor;
	gPlayer[data.index].room = data.room;
	memcpy(gPlayer[data.index].weapons, data.weapons, sizeof(char) * 8);
	memcpy(gPlayer[data.index].ammo, data.ammo, sizeof(short) * 8);
	memcpy(gPlayer[data.index].color, data.color, sizeof(unsigned char) * 4);

	HOOK.PlayerConnect(data.name, data.index, gPlayer[data.index].model, gPlayer[data.index].position[0], gPlayer[data.index].position[1], gPlayer[data.index].position[2]);
}

void NetworkManager::RecieveClientConnectionError(NetworkPlayerConnectionErrorData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	switch (data.error)
	{
	case NetworkPlayerConnectionErrorServerFull:
		{
			PrintToConsole("Connection error: Server is full.");
			break;
		}
	case NetworkPlayerConnectionErrorInvalidProtocol:
		{
			PrintToConsole("Connection error: Server is using different protocol.");
			break;
		}
	case NetworkPlayerConnectionErrorInvalidName:
		{
			PrintToConsole("Connection error: Invalid user name.");
			break;
		}
	case NetworkPlayerConnectionErrorAlreadyConnected:
		{
			PrintToConsole("Connection error: You are already connected.");
			break;
		}
	case NetworkPlayerConnectionErrorAllocationError:
		{
			PrintToConsole("Connection error: Server was unable to allocate player resources.");
			break;
		}
	case NetworkPlayerConnectionErrorScriptLock:
		{
			PrintToConsole("Connection error: Connection has been refused by a server script.");
			break;
		}
	}
}

void NetworkManager::RecieveClientInfo(NetworkPlayerInfoData data, RakNet::RPC3 *serverrpc3)
{
	if (data.sessionkey != client.GetSessionKey())
	{
		return;
	}
	clientid.localSystemAddress = data.index + 1;
	this->SetNetworkID(clientid);
	client.SetIndex(data.index);
}

void NetworkManager::RecieveClientDisconnection(NetworkPlayerDisconnectionData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.PlayerDisconnect(data.client);
}

void NetworkManager::RecievePlayerFullUpdate(NetworkPlayerFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	//TODO: Do NOT load at connect, redo the whole thing.
	gPlayer[data.index].model = data.model;
	memcpy(gPlayer[data.index].position, data.position, sizeof(float) * 3);
	gPlayer[data.index].angle = data.angle;
	gPlayer[data.index].vehicleindex = data.vehicleindex;
	gPlayer[data.index].seatindex = data.seatindex;
	gPlayer[data.index].score = data.score;
	gPlayer[data.index].health = data.health;
	gPlayer[data.index].armor = data.armor;
	gPlayer[data.index].room = data.room;
	memcpy(gPlayer[data.index].weapons, data.weapons, sizeof(char) * 8);
	memcpy(gPlayer[data.index].ammo, data.ammo, sizeof(short) * 8);
	memcpy(gPlayer[data.index].color, data.color, sizeof(unsigned char) * 4);

	Log("Player full update. Name is %s", data.name);
	HOOK.PlayerConnect(data.name, data.index, gPlayer[data.index].model, gPlayer[data.index].position[0], gPlayer[data.index].position[1], gPlayer[data.index].position[2]);
}

void NetworkManager::RecieveVehicleFullUpdate(NetworkVehicleFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	//TODO: Do NOT load at connect, redo the whole thing.
	HOOK.CreateCar(data.index, data.model, data.position[0], data.position[1], data.position[2], data.angle, data.color[0], data.color[2]);
}

void NetworkManager::RecievePlayerMove(NetworkPlayerMoveData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	if(gPlayer[data.client].vehicleindex != -1)
	{
		memcpy(gCar[gPlayer[data.client].vehicleindex].position, data.position, sizeof(float) * 3);
		gCar[gPlayer[data.client].vehicleindex].angle = data.angle;
	}

	memcpy(gPlayer[data.client].position, data.position, sizeof(float) * 3);
	gPlayer[data.client].angle = data.angle;

	HOOK.PlayerMove(data.client, data.position[0], data.position[1], data.position[2], data.speed);
}

void NetworkManager::RecievePlayerJump(NetworkPlayerJumpData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.Jump(data.client);
}

void NetworkManager::RecievePlayerDuck(NetworkPlayerDuckData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.Duck(data.client, data.shouldduck);
}

void NetworkManager::RecievePlayerEntranceInVehicle(NetworkPlayerEntranceInVehicleData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.EnterInVehicle(data.client, data.vehicleindex, data.seat);
}

void NetworkManager::RecievePlayerCancelEntranceInVehicle(NetworkPlayerCancelEntranceInVehicleData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.CancelEnterInVehicle(data.client);
}

void NetworkManager::RecievePlayerExitFromVehicle(NetworkPlayerExitFromVehicleData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.ExitFromVehicle(data.client);
}

void NetworkManager::RecievePlayerFire(NetworkPlayerFireData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.PlayerFireAim(data.client, data.weapon, data.time, data.position[0], data.position[1], data.position[2], 1);
}

void NetworkManager::RecievePlayerAim(NetworkPlayerAimData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.PlayerFireAim(data.client, data.weapon, data.time, data.position[0], data.position[1], data.position[2], 0);
}

void NetworkManager::RecievePlayerWeaponChange(NetworkPlayerWeaponChangeData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.PlayerSwapGun(data.client, data.weapon);
}

void NetworkManager::RecievePlayerHealthAndArmorChange(NetworkPlayerHealthAndArmorChangeData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	// ... Do it ...
}

void NetworkManager::RecievePlayerSpawn(NetworkPlayerSpawnData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.xPlayerSpawn(data);
}

void NetworkManager::RecievePlayerModelChange(NetworkPlayerModelChangeData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.PlayerSyncSkin(data.client, data.model);
}

void NetworkManager::RecievePlayerComponentsChange(NetworkPlayerComponentsChangeData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.PlayerSyncSkinVariation(data.client, data.compD, data.compT);
}

void NetworkManager::RecievePlayerChat(NetworkPlayerChatData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	AddChatMessage(data.msg, COLOR(data.color[0], data.color[1], data.color[2]), data.client);
}

void NetworkManager::RecieveNewVehicle(NetworkVehicleFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
	LastUpdate = GetTickCount();
	HOOK.CreateCar(data.index, data.model, data.position[0], data.position[1], data.position[2], data.angle, data.color[0], data.color[2]);
}