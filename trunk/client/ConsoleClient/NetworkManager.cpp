#include "RakNetworkFactory.h"
#include "MessageIdentifiers.h"
#include "GetTime.h"

#include "NetworkManager.h"
#include "ClientCore.h"
#include "../../Shared/Console/ConsoleScreen.h"

extern ClientCore client;
extern ConsoleScreen conscreen;

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
	clientid.localSystemAddress = 65534;
	serverid.localSystemAddress = 0;
	this->SetNetworkIDManager(manager);
	this->SetNetworkID(clientid);
	rpc3 = new RakNet::RPC3;
	rpc3->SetNetworkIDManager(manager);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecieveClientConnection);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecieveClientConnectionError);
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
		switch(pack->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				conscreen.Print("RakNet: Connection accepted. Sending client info...");
				this->SendClientConnectionRequest();
			} break;
		case ID_ALREADY_CONNECTED:
			{
				conscreen.Print("RakNet: Already connected");
			} break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			{
				conscreen.Print("RakNet: No free connections");
			} break;
		case ID_DISCONNECTION_NOTIFICATION:
			{
				conscreen.Print("RakNet: Disconnect (Close connection)");
			} break;
		case ID_CONNECTION_LOST:
			{
				conscreen.Print("RakNet: Disconnect (Connection lost)");
			} break;
		case ID_CONNECTION_BANNED:
			{
				conscreen.Print("RakNet: Disconnect (Connection banned)");
			} break;
		case ID_INVALID_PASSWORD:
			{
				conscreen.Print("RakNet: Invalid password");
			} break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			{
				conscreen.Print("RakNet: Connection failed");
			} break;
		case ID_PONG:
			{
				conscreen.Print("RakNet: Pong");
			} break;
		case ID_RPC_REMOTE_ERROR:
			{
				conscreen.Print("RakNet: RPC remote error");
				switch (pack->data[1])
				{
				case RakNet::RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE:
					conscreen.Print("RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE\n");
					break;
				case RakNet::RPC_ERROR_OBJECT_DOES_NOT_EXIST:
					conscreen.Print("RPC_ERROR_OBJECT_DOES_NOT_EXIST\n");
					break;
				case RakNet::RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE:
					conscreen.Print("RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE\n");
					break;
				case RakNet::RPC_ERROR_FUNCTION_NOT_REGISTERED:
					conscreen.Print("RPC_ERROR_FUNCTION_NOT_REGISTERED\n");
					break;
				case RakNet::RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED:
					conscreen.Print("RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED\n");
					break;
				case RakNet::RPC_ERROR_CALLING_CPP_AS_C:
					conscreen.Print("RPC_ERROR_CALLING_CPP_AS_C\n");
					break;
				case RakNet::RPC_ERROR_CALLING_C_AS_CPP:
					conscreen.Print("RPC_ERROR_CALLING_C_AS_CPP\n");
					break;
				}
			} break;
		default:
			{
				conscreen.Print("RakNet: Unknown message (0x%x) [%s]", pack->data[0], pack->data);
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
	// Коннект
	net->Connect(hostname, port, 0, 0, 0);
	serveraddress.SetBinaryAddress(hostname);
	serveraddress.port = port;
	conscreen.Print("Connecting to server...");
}

void NetworkManager::SendClientConnectionRequest(void)
{
	NetworkPlayerConnectionRequestData data;
	data.protocol = PROTOCOL_VERSION;
	char *name = client.GetName();
	strcpy(data.name, name);
	free(name);
	rpc3->CallCPP("&NetworkManager::RecieveClientConnectionRequest", serverid, data, rpc3);
}

void NetworkManager::SendPlayerMove(const float speed)
{
	NetworkPlayerMoveData data;
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
	rpc3->CallCPP("&NetworkManager::RecievePlayerDuck", serverid, data, rpc3);
}

void NetworkManager::SendPlayerEntranceInVehicle(const char seat)
{
	NetworkPlayerEntranceInVehicleData data;
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
	rpc3->CallCPP("&NetworkManager::RecievePlayerFire", serverid, data, rpc3);
}

void NetworkManager::SendPlayerAim(const float position[3], const int time)
{
	NetworkPlayerAimData data;
	rpc3->CallCPP("&NetworkManager::RecievePlayerAim", serverid, data, rpc3);
}

void NetworkManager::SendPlayerWeaponChange(void)
{
	NetworkPlayerWeaponChangeData data;
	rpc3->CallCPP("&NetworkManager::RecievePlayerWeaponChange", serverid, data, rpc3);
}

void NetworkManager::SendPlayerHealthAndArmorChange(void)
{
	NetworkPlayerHealthAndArmorChangeData data;
	rpc3->CallCPP("&NetworkManager::RecievePlayerHealthAndArmorChange", serverid, data, rpc3);
}

void NetworkManager::SendPlayerSpawnRequest(void)
{
}

void NetworkManager::SendPlayerModelChange(void)
{
}

void NetworkManager::SendPlayerComponentsChange(void)
{
	NetworkPlayerComponentsChangeData data;
	rpc3->CallCPP("&NetworkManager::RecievePlayerComponentsChange", serverid, data, rpc3);
}

void NetworkManager::SendPlayerChat(void)
{
	NetworkPlayerChatData data;
	rpc3->CallCPP("&NetworkManager::RecievePlayerChat", serverid, data, rpc3);
}

void NetworkManager::RecieveClientConnection(NetworkPlayerFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
	conscreen.Print("Recieving player info");
	clientid.localSystemAddress = data.index + 1;
	this->SetNetworkID(clientid);
	conscreen.Print("Name is %s", data.name);
}

void NetworkManager::RecieveClientConnectionError(NetworkPlayerConnectionErrorData data, RakNet::RPC3 *serverrpc3)
{
	switch (data.error)
	{
	case NetworkPlayerConnectionErrorServerFull:
		{
			conscreen.Print("ConnectError: Server is full.");
			break;
		}
	case NetworkPlayerConnectionErrorInvalidProtocol:
		{
			conscreen.Print("ConnectError: Server is using different protocol.");
			break;
		}
	case NetworkPlayerConnectionErrorAlreadyConnected:
		{
			conscreen.Print("ConnectError: You are already connected.");
			break;
		}
	case NetworkPlayerConnectionErrorAllocationError:
		{
			conscreen.Print("ConnectError: Server was unable to allocate player resources.");
			break;
		}
	case NetworkPlayerConnectionErrorScriptLock:
		{
			conscreen.Print("ConnectError: Script lock connect");
			break;
		}
	}
}

void NetworkManager::RecieveClientDisconnection(NetworkPlayerDisconnectionData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerFullUpdate(NetworkPlayerFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
	conscreen.Print("Name is %s", data.name);
}

void NetworkManager::RecieveVehicleFullUpdate(NetworkVehicleFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
	conscreen.Print("Recieving vehicle info");
}

void NetworkManager::RecievePlayerMove(NetworkPlayerMoveData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerJump(NetworkPlayerJumpData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerDuck(NetworkPlayerDuckData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerEntranceInVehicle(NetworkPlayerEntranceInVehicleData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerCancelEntranceInVehicle(NetworkPlayerCancelEntranceInVehicleData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerExitFromVehicle(NetworkPlayerExitFromVehicleData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerFire(NetworkPlayerFireData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerAim(NetworkPlayerAimData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerWeaponChange(NetworkPlayerWeaponChangeData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerHealthAndArmorChange(NetworkPlayerHealthAndArmorChangeData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerSpawn(NetworkPlayerSpawnData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerModelChange(NetworkPlayerModelChangeData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerComponentsChange(NetworkPlayerComponentsChangeData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecievePlayerChat(NetworkPlayerChatData data, RakNet::RPC3 *serverrpc3)
{
}

void NetworkManager::RecieveNewVehicle(NetworkVehicleFullUpdateData data, RakNet::RPC3 *serverrpc3)
{
}