#include "RakNetworkFactory.h"
#include "MessageIdentifiers.h"
#include "GetTime.h"

#include "NetworkManager.h"
#include "main.h"
#include "../../Shared/Console/common.h"
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
extern char enterMsg[256];

NetworkManager::NetworkManager(void)
{
	manager = NULL;
	rpc3 = NULL;
	net = NULL;
	maxrpcbuffersize = 2147483647;
	rpcbuffer = NULL;
	rpcbuffersize = 0;
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
	this->FreeRPCBuffer();
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
	InitializeCriticalSection(&rpcbuffersection);
}

void NetworkManager::Tick(void)
{
	EnterCriticalSection(&rpcbuffersection);
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
	if (rpcbuffer != NULL)
	{
		for (int i = 0; i < rpcbuffersize; i++)
		{
			this->HandleRPCData(rpcbuffer[i].type, &rpcbuffer[i].data);
		}
		free(rpcbuffer);
		rpcbuffer = NULL;
		rpcbuffersize = 0;
	}
	LeaveCriticalSection(&rpcbuffersection);
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
	this->FreeRPCBuffer();
	DeleteCriticalSection(&rpcbuffersection);
}

void NetworkManager::Ping(const char *hostname, const unsigned short port)
{
	net->Ping(hostname, port, false);
}

void NetworkManager::ConnectToServer(const char *hostname, const unsigned short port)
{
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
	this->WriteToRPCBuffer(NetworkRPCPlayerConnection, &data);
}

void NetworkManager::RecieveClientConnectionError(NetworkPlayerConnectionErrorData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerConnectionError, &data);
}

void NetworkManager::RecieveClientInfo(NetworkPlayerInfoData data, RakNet::RPC3 *serverrpc3)
{
	//this->WriteToRPCBuffer(NetworkRPCPlayerInfo, &data);
	//Temp fix. Client should use rpc buffer and then send confirmation to the server.
	//After that, server should send full update.
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
	this->WriteToRPCBuffer(NetworkRPCPlayerDisconnection, &data);
}

void NetworkManager::RecievePlayerFullUpdate(NetworkPlayerFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerFullUpdate, &data);
}

void NetworkManager::RecieveVehicleFullUpdate(NetworkVehicleFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCVehicleFullUpdate, &data);
}

void NetworkManager::RecievePlayerMove(NetworkPlayerMoveData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerMove, &data);
}

void NetworkManager::RecievePlayerJump(NetworkPlayerJumpData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerJump, &data);
}

void NetworkManager::RecievePlayerDuck(NetworkPlayerDuckData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerDuck, &data);
}

void NetworkManager::RecievePlayerEntranceInVehicle(NetworkPlayerEntranceInVehicleData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerEntranceInVehicle, &data);
}

void NetworkManager::RecievePlayerCancelEntranceInVehicle(NetworkPlayerCancelEntranceInVehicleData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerCancelEntranceInVehicle, &data);
}

void NetworkManager::RecievePlayerExitFromVehicle(NetworkPlayerExitFromVehicleData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerExitFromVehicle, &data);
}

void NetworkManager::RecievePlayerFire(NetworkPlayerFireData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerFire, &data);
}

void NetworkManager::RecievePlayerAim(NetworkPlayerAimData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerAim, &data);
}

void NetworkManager::RecievePlayerWeaponChange(NetworkPlayerWeaponChangeData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerWeaponChange, &data);
}

void NetworkManager::RecievePlayerHealthAndArmorChange(NetworkPlayerHealthAndArmorChangeData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerHealthAndArmorChange, &data);
}

void NetworkManager::RecievePlayerSpawn(NetworkPlayerSpawnData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerSpawn, &data);
}

void NetworkManager::RecievePlayerModelChange(NetworkPlayerModelChangeData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerModelChange, &data);
}

void NetworkManager::RecievePlayerComponentsChange(NetworkPlayerComponentsChangeData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerComponentsChange, &data);
}

void NetworkManager::RecievePlayerChat(NetworkPlayerChatData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCPlayerChat, &data);
}

void NetworkManager::RecieveNewVehicle(NetworkVehicleFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
	this->WriteToRPCBuffer(NetworkRPCNewVehicle, &data);
}

template <typename DATATYPE>
void NetworkManager::WriteToRPCBuffer(const NetworkManager::NetworkRPCType type, const DATATYPE *data)
{
	if (rpcbuffersize == maxrpcbuffersize)
	{
		return;
	}
	EnterCriticalSection(&rpcbuffersection);
	if (!ResizeBuffer<NetworkRPCData *>(rpcbuffer, rpcbuffersize + 1))
	{
		return;
	}
	rpcbuffer[rpcbuffersize].type = type;
	switch (type)
	{
	case NetworkRPCPlayerConnection:
		{
			rpcbuffer[rpcbuffersize].data.playerconnection = (NetworkPlayerFullUpdateData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerconnection, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerConnectionError:
		{
			rpcbuffer[rpcbuffersize].data.playerconnectionerror = (NetworkPlayerConnectionErrorData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerconnectionerror, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerInfo:
		{
			rpcbuffer[rpcbuffersize].data.playerinfo = (NetworkPlayerInfoData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerinfo, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerDisconnection:
		{
			rpcbuffer[rpcbuffersize].data.playerdisconnection = (NetworkPlayerDisconnectionData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerdisconnection, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerFullUpdate:
		{
			rpcbuffer[rpcbuffersize].data.playerfullupdate = (NetworkPlayerFullUpdateData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerfullupdate, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCVehicleFullUpdate:
		{
			rpcbuffer[rpcbuffersize].data.vehiclefullupdate = (NetworkVehicleFullUpdateData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.vehiclefullupdate, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerMove:
		{
			rpcbuffer[rpcbuffersize].data.playermove = (NetworkPlayerMoveData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playermove, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerJump:
		{
			rpcbuffer[rpcbuffersize].data.playerjump = (NetworkPlayerJumpData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerjump, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerDuck:
		{
			rpcbuffer[rpcbuffersize].data.playerduck = (NetworkPlayerDuckData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerduck, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerEntranceInVehicle:
		{
			rpcbuffer[rpcbuffersize].data.playerentranceinvehicle = (NetworkPlayerEntranceInVehicleData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerentranceinvehicle, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerCancelEntranceInVehicle:
		{
			rpcbuffer[rpcbuffersize].data.playercancelentranceinvehicle = (NetworkPlayerCancelEntranceInVehicleData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playercancelentranceinvehicle, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerExitFromVehicle:
		{
			rpcbuffer[rpcbuffersize].data.playerexitfromvehicle = (NetworkPlayerExitFromVehicleData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerexitfromvehicle, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerFire:
		{
			rpcbuffer[rpcbuffersize].data.playerfire = (NetworkPlayerFireData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerfire, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerAim:
		{
			rpcbuffer[rpcbuffersize].data.playeraim = (NetworkPlayerAimData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playeraim, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerWeaponChange:
		{
			rpcbuffer[rpcbuffersize].data.playerweaponchange = (NetworkPlayerWeaponChangeData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerweaponchange, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerHealthAndArmorChange:
		{
			rpcbuffer[rpcbuffersize].data.playerhealthandarmorchange = (NetworkPlayerHealthAndArmorChangeData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerhealthandarmorchange, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerSpawn:
		{
			rpcbuffer[rpcbuffersize].data.playerspawn = (NetworkPlayerSpawnData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerspawn, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerModelChange:
		{
			rpcbuffer[rpcbuffersize].data.playermodelchange = (NetworkPlayerModelChangeData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playermodelchange, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerComponentsChange:
		{
			rpcbuffer[rpcbuffersize].data.playercomponentschange = (NetworkPlayerComponentsChangeData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playercomponentschange, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerChat:
		{
			rpcbuffer[rpcbuffersize].data.playerchat = (NetworkPlayerChatData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerchat, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCNewVehicle:
		{
			rpcbuffer[rpcbuffersize].data.newvehicle = (NetworkVehicleFullUpdateData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.newvehicle, data, sizeof(DATATYPE));
			break;
		}
	}
	rpcbuffersize++;
	LeaveCriticalSection(&rpcbuffersection);
}

void NetworkManager::HandleRPCData(const NetworkRPCType type, const NetworkRPCUnion *data)
{
	if (data == NULL)
	{
		return;
	}
	switch (type)
	{
	case NetworkRPCPlayerConnection:
		{
			Log("New player connection. Name is %s", data->playerconnection->name);
			gPlayer[data->playerconnection->index].model = data->playerconnection->model;
			memcpy(gPlayer[data->playerconnection->index].position, data->playerconnection->position, sizeof(float) * 3);
			gPlayer[data->playerconnection->index].angle = data->playerconnection->angle;
			gPlayer[data->playerconnection->index].vehicleindex = data->playerconnection->vehicleindex;
			gPlayer[data->playerconnection->index].seatindex = data->playerconnection->seatindex;
			gPlayer[data->playerconnection->index].score = data->playerconnection->score;
			gPlayer[data->playerconnection->index].health = data->playerconnection->health;
			gPlayer[data->playerconnection->index].armor = data->playerconnection->armor;
			gPlayer[data->playerconnection->index].room = data->playerconnection->room;
			memcpy(gPlayer[data->playerconnection->index].weapons, data->playerconnection->weapons, sizeof(char) * 8);
			memcpy(gPlayer[data->playerconnection->index].ammo, data->playerconnection->ammo, sizeof(short) * 8);
			memcpy(gPlayer[data->playerconnection->index].color, data->playerconnection->color, sizeof(unsigned char) * 4);
			HOOK.PlayerConnect(data->playerconnection->name, data->playerconnection->index, gPlayer[data->playerconnection->index].model, gPlayer[data->playerconnection->index].position);
			delete data->playerconnection;
			break;
		}
	case NetworkRPCPlayerConnectionError:
		{
			switch (data->playerconnectionerror->error)
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
			delete data->playerconnectionerror;
			break;
		}
	case NetworkRPCPlayerInfo:
		{
			if (data->playerinfo->sessionkey != client.GetSessionKey())
			{
				return;
			}
			clientid.localSystemAddress = data->playerinfo->index + 1;
			this->SetNetworkID(clientid);
			client.SetIndex(data->playerinfo->index);
			delete data->playerinfo;
			break;
		}
	case NetworkRPCPlayerDisconnection:
		{
			HOOK.PlayerDisconnect(data->playerdisconnection->client);
			delete data->playerdisconnection;
			break;
		}
	case NetworkRPCPlayerFullUpdate:
		{
			//TODO: Do NOT load at connect, redo the whole thing.
			gPlayer[data->playerfullupdate->index].model = data->playerfullupdate->model;
			memcpy(gPlayer[data->playerfullupdate->index].position, data->playerfullupdate->position, sizeof(float) * 3);
			gPlayer[data->playerfullupdate->index].angle = data->playerfullupdate->angle;
			gPlayer[data->playerfullupdate->index].vehicleindex = data->playerfullupdate->vehicleindex;
			gPlayer[data->playerfullupdate->index].seatindex = data->playerfullupdate->seatindex;
			gPlayer[data->playerfullupdate->index].score = data->playerfullupdate->score;
			gPlayer[data->playerfullupdate->index].health = data->playerfullupdate->health;
			gPlayer[data->playerfullupdate->index].armor = data->playerfullupdate->armor;
			gPlayer[data->playerfullupdate->index].room = data->playerfullupdate->room;
			memcpy(gPlayer[data->playerfullupdate->index].weapons, data->playerfullupdate->weapons, sizeof(char) * 8);
			memcpy(gPlayer[data->playerfullupdate->index].ammo, data->playerfullupdate->ammo, sizeof(short) * 8);
			memcpy(gPlayer[data->playerfullupdate->index].color, data->playerfullupdate->color, sizeof(unsigned char) * 4);

			Log("Player full update. Name is %s", data->playerfullupdate->name);
			HOOK.PlayerConnect(data->playerfullupdate->name, data->playerfullupdate->index, gPlayer[data->playerfullupdate->index].model, gPlayer[data->playerfullupdate->index].position);
			delete data->playerfullupdate;
			break;
		}
	case NetworkRPCVehicleFullUpdate:
		{
			//TODO: Do NOT load at connect, redo the whole thing.
			HOOK.CreateCar(data->vehiclefullupdate->index, data->vehiclefullupdate->model, data->vehiclefullupdate->position, data->vehiclefullupdate->angle, data->vehiclefullupdate->color);
			delete data->vehiclefullupdate;
			break;
		}
	case NetworkRPCPlayerMove:
		{
			if(gPlayer[data->playermove->client].vehicleindex != -1)
			{
				memcpy(gCar[gPlayer[data->playermove->client].vehicleindex].position, data->playermove->position, sizeof(float) * 3);
				gCar[gPlayer[data->playermove->client].vehicleindex].angle = data->playermove->angle;
			}

			memcpy(gPlayer[data->playermove->client].position, data->playermove->position, sizeof(float) * 3);
			gPlayer[data->playermove->client].angle = data->playermove->angle;

			HOOK.PlayerMove(data->playermove->client, data->playermove->position, data->playermove->speed);
			delete data->playermove;
			break;
		}
	case NetworkRPCPlayerJump:
		{
			HOOK.Jump(data->playerjump->client);
			delete data->playerjump;
			break;
		}
	case NetworkRPCPlayerDuck:
		{
			HOOK.Duck(data->playerduck->client, data->playerduck->shouldduck);
			delete data->playerduck;
			break;
		}
	case NetworkRPCPlayerEntranceInVehicle:
		{
			HOOK.EnterInVehicle(data->playerentranceinvehicle->client, data->playerentranceinvehicle->vehicleindex, data->playerentranceinvehicle->seat);
			delete data->playerentranceinvehicle;
			break;
		}
	case NetworkRPCPlayerCancelEntranceInVehicle:
		{
			HOOK.CancelEnterInVehicle(data->playercancelentranceinvehicle->client);
			delete data->playercancelentranceinvehicle;
			break;
		}
	case NetworkRPCPlayerExitFromVehicle:
		{
			HOOK.ExitFromVehicle(data->playerexitfromvehicle->client);
			delete data->playerexitfromvehicle;
			break;
		}
	case NetworkRPCPlayerFire:
		{
			HOOK.PlayerFireAim(data->playerfire->client, data->playerfire->weapon, data->playerfire->time, data->playerfire->position[0], data->playerfire->position[1], data->playerfire->position[2], 1);
			delete data->playerfire;
			break;
		}
	case NetworkRPCPlayerAim:
		{
			HOOK.PlayerFireAim(data->playeraim->client, data->playeraim->weapon, data->playeraim->time, data->playeraim->position[0], data->playeraim->position[1], data->playeraim->position[2], 0);
			delete data->playeraim;
			break;
		}
	case NetworkRPCPlayerWeaponChange:
		{
			HOOK.PlayerSwapGun(data->playerweaponchange->client, data->playerweaponchange->weapon);
			delete data->playerweaponchange;
			break;
		}
	case NetworkRPCPlayerHealthAndArmorChange:
		{
			delete data->playerhealthandarmorchange;
			break;
		}
	case NetworkRPCPlayerSpawn:
		{
			HOOK.xPlayerSpawn(*(data->playerspawn));
			delete data->playerspawn;
			break;
		}
	case NetworkRPCPlayerModelChange:
		{
			HOOK.PlayerSyncSkin(data->playermodelchange->client, data->playermodelchange->model);
			delete data->playermodelchange;
			break;
		}
	case NetworkRPCPlayerComponentsChange:
		{
			HOOK.PlayerSyncSkinVariation(data->playercomponentschange->client, data->playercomponentschange->compD, data->playercomponentschange->compT);
			delete data->playercomponentschange;
			break;
		}
	case NetworkRPCPlayerChat:
		{
			AddChatMessage(data->playerchat->msg, COLOR(data->playerchat->color[0], data->playerchat->color[1], data->playerchat->color[2]), data->playerchat->client);
			delete data->playerchat;
			break;
		}
	case NetworkRPCNewVehicle:
		{
			HOOK.CreateCar(data->newvehicle->index, data->newvehicle->model, data->newvehicle->position, data->newvehicle->angle, data->newvehicle->color);
			delete data->newvehicle;
			break;
		}
	}
}

void NetworkManager::FreeRPCBuffer(void)
{
	if (rpcbuffer == NULL)
	{
		return;
	}
	for (int i = 0; i < rpcbuffersize; i++)
	{
		switch (rpcbuffer[i].type)
		{
		case NetworkRPCPlayerConnection:
			{
				delete rpcbuffer[i].data.playerconnection;
				break;
			}
		case NetworkRPCPlayerConnectionError:
			{
				delete rpcbuffer[i].data.playerconnectionerror;
				break;
			}
		case NetworkRPCPlayerInfo:
			{
				delete rpcbuffer[i].data.playerinfo;
				break;
			}
		case NetworkRPCPlayerDisconnection:
			{
				delete rpcbuffer[i].data.playerdisconnection;
				break;
			}
		case NetworkRPCPlayerFullUpdate:
			{
				delete rpcbuffer[i].data.playerfullupdate;
				break;
			}
		case NetworkRPCVehicleFullUpdate:
			{
				delete rpcbuffer[i].data.vehiclefullupdate;
				break;
			}
		case NetworkRPCPlayerMove:
			{
				delete rpcbuffer[i].data.playermove;
				break;
			}
		case NetworkRPCPlayerJump:
			{
				delete rpcbuffer[i].data.playerjump;
				break;
			}
		case NetworkRPCPlayerDuck:
			{
				delete rpcbuffer[i].data.playerduck;
				break;
			}
		case NetworkRPCPlayerEntranceInVehicle:
			{
				delete rpcbuffer[i].data.playerentranceinvehicle;
				break;
			}
		case NetworkRPCPlayerCancelEntranceInVehicle:
			{
				delete rpcbuffer[i].data.playercancelentranceinvehicle;
				break;
			}
		case NetworkRPCPlayerExitFromVehicle:
			{
				delete rpcbuffer[i].data.playerexitfromvehicle;
				break;
			}
		case NetworkRPCPlayerFire:
			{
				delete rpcbuffer[i].data.playerfire;
				break;
			}
		case NetworkRPCPlayerAim:
			{
				delete rpcbuffer[i].data.playeraim;
				break;
			}
		case NetworkRPCPlayerWeaponChange:
			{
				delete rpcbuffer[i].data.playerweaponchange;
				break;
			}
		case NetworkRPCPlayerHealthAndArmorChange:
			{
				delete rpcbuffer[i].data.playerhealthandarmorchange;
				break;
			}
		case NetworkRPCPlayerSpawn:
			{
				delete rpcbuffer[i].data.playerspawn;
				break;
			}
		case NetworkRPCPlayerModelChange:
			{
				delete rpcbuffer[i].data.playermodelchange;
				break;
			}
		case NetworkRPCPlayerComponentsChange:
			{
				delete rpcbuffer[i].data.playercomponentschange;
				break;
			}
		case NetworkRPCPlayerChat:
			{
				delete rpcbuffer[i].data.playerchat;
				break;
			}
		case NetworkRPCNewVehicle:
			{
				delete rpcbuffer[i].data.newvehicle;
				break;
			}
		}
	}
	free(rpcbuffer);
	rpcbuffer = NULL;
	rpcbuffersize = 0;
}