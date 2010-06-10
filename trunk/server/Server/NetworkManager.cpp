#include "../../Shared/RPC3/RPC3.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include "MessageIdentifiers.h"
#include "MasterServerManager.h"

#include "NetworkManager.h"
#include "../../Shared/Console/common.h"
#include "logging.h"
#include "ServerCore.h"
#include "VirtualMachineManager.h"
#include "PlayerManager.h"
#include "VehicleManager.h"

extern ServerCore server;
extern MasterServerManager msm;
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
	maxrpcbuffersize = 2147483647;
	rpcbuffer = NULL;
	rpcbuffersize = 0;
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
	this->FreeRPCBuffer();
}

void NetworkManager::Load(const short maxclients, const unsigned short port)
{
	manager = new NetworkIDManager;
	manager->SetIsNetworkIDAuthority(true);
	serverid.localSystemAddress = DEFAULT_SERVER_NETWORK_ID;
	defaultclientid.localSystemAddress = DEFAULT_CLIENT_NETWORK_ID;
	this->SetNetworkIDManager(manager);
	this->SetNetworkID(serverid);
	rpc3 = new RakNet::RPC3;
	rpc3->SetNetworkIDManager(manager);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecieveClientConnectionRequest);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecieveClientConnectionConfirmation);
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
#ifdef WIN32
	InitializeCriticalSection(&rpcbuffersection);
#endif
}

void NetworkManager::Tick(void)
{
#ifdef WIN32
	EnterCriticalSection(&rpcbuffersection);
#endif
	static Packet *pack;
	for (pack = net->Receive(); pack; net->DeallocatePacket(pack), pack = net->Receive())
	{
		switch (pack->data[0])
		{
		case ID_CONNECTION_REQUEST:
			{
				debug(L"New connection request");
				break;
			}
		case ID_NEW_INCOMING_CONNECTION:
			{
				debug(L"New connection from %S:%d", pack->systemAddress.ToString(0), pack->systemAddress.port);
				break;
			}
		case ID_DISCONNECTION_NOTIFICATION:
			{
				this->HandleClientDisconnection(pack->systemAddress);
				break;
			}
		case ID_CONNECTION_LOST:
			{
				this->HandleClientDisconnection(pack->systemAddress);
				break;
			}
		case ID_RPC_REMOTE_ERROR:
			{
				PrintToServer(L"RakNet: RPC remote error");
				switch (pack->data[1])
				{
				case RakNet::RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE:
					PrintToServer(L"RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE\n");
					break;
				case RakNet::RPC_ERROR_OBJECT_DOES_NOT_EXIST:
					PrintToServer(L"RPC_ERROR_OBJECT_DOES_NOT_EXIST\n");
					break;
				case RakNet::RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE:
					PrintToServer(L"RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE\n");
					break;
				case RakNet::RPC_ERROR_FUNCTION_NOT_REGISTERED:
					PrintToServer(L"RPC_ERROR_FUNCTION_NOT_REGISTERED\n");
					break;
				case RakNet::RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED:
					PrintToServer(L"RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED\n");
					break;
				case RakNet::RPC_ERROR_CALLING_CPP_AS_C:
					PrintToServer(L"RPC_ERROR_CALLING_CPP_AS_C\n");
					break;
				case RakNet::RPC_ERROR_CALLING_C_AS_CPP:
					PrintToServer(L"RPC_ERROR_CALLING_C_AS_CPP\n");
					break;
				}
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
#ifdef WIN32
	LeaveCriticalSection(&rpcbuffersection);
#endif
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
	this->FreeRPCBuffer();
#ifdef WIN32
	DeleteCriticalSection(&rpcbuffersection);
#endif
}

void NetworkManager::CheckClients(void)
{
	//this->SendDataToAll("&NetworkManager::RecieveCheck", );
}

void NetworkManager::UpdateServerInfo(void)
{
	wchar_t *hostname = server.GetHostname();
	wchar_t *gamemode = vmm.GetGameModeName();
	int length = _scwprintf(L"%s%c%s%c%s%c%d%c%d%c%d%c%s%c", hostname, 1, gamemode, 1, L"World", 1, playm.numplayers, 1, playm.maxplayerbuffersize, 1, server.IsPasswordProtected(), 1, L"", 1);
	wchar_t *tempstring = (wchar_t *)calloc(length + 1, sizeof(wchar_t));
	swprintf(tempstring, length + 1, L"%s%c%s%c%s%c%d%c%d%c%d%c%s%c", hostname, 1, gamemode, 1, L"World", 1, playm.numplayers, 1, playm.maxplayerbuffersize, 1, server.IsPasswordProtected(), 1, L"", 1);
	length = (sizeof(wchar_t) / sizeof(char)) * length + 1;
	char *info = (char *)calloc(length, sizeof(char));
	wcstombs(info, tempstring, length);
	net->SetOfflinePingResponse(info, length);
	free(hostname);
	free(gamemode);
	free(info);
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
		PrintToServer(L"Client has different protocol version.");
		this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorInvalidProtocol);
		net->CloseConnection(clientrpc3->GetLastSenderAddress(), true);
		return;
	}
	if (wcslen(data.name) == 0)
	{
		PrintToServer(L"Client has invalid name.");
		this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorInvalidName);
		net->CloseConnection(clientrpc3->GetLastSenderAddress(), true);
		return;
	}
	if (!server.IsLAN())
	{
		if(!msm.QueryUserCheck(data.fmpid, clientrpc3->GetLastSenderAddress().ToString(0), data.sessionkey))
		{
			PrintToServer(L"Client has failed to pass an authentication.");
			this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorAlreadyConnected);
			net->CloseConnection(clientrpc3->GetLastSenderAddress(), true);
			return;
		}
	}
	else
	{
		// TODO: Offline name check
	}

	NetworkPlayerConnectionRequestDataInternal data2;
	data2.address = clientrpc3->GetLastSenderAddress();
	wcscpy(data2.name, data.name);
	data2.sessionkey = data.sessionkey;
	this->WriteToRPCBuffer(NetworkRPCPlayerConnectionRequest, &data2);
}

void NetworkManager::RecieveClientConnectionConfirmation(NetworkPlayerConnectionConfirmationData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerConnectionConfirmation, &data);
}

void NetworkManager::RecievePlayerMove(NetworkPlayerMoveData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerMove, &data);
}

void NetworkManager::RecievePlayerJump(NetworkPlayerJumpData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerJump, &data);
}

void NetworkManager::RecievePlayerDuck(NetworkPlayerDuckData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerDuck, &data);
}

void NetworkManager::RecievePlayerEntranceInVehicle(NetworkPlayerEntranceInVehicleData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerEntranceInVehicle, &data);
}

void NetworkManager::RecievePlayerCancelEntranceInVehicle(NetworkPlayerCancelEntranceInVehicleData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerCancelEntranceInVehicle, &data);
}

void NetworkManager::RecievePlayerExitFromVehicle(NetworkPlayerExitFromVehicleData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerExitFromVehicle, &data);
}

void NetworkManager::RecievePlayerFire(NetworkPlayerFireData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerFire, &data);
}

void NetworkManager::RecievePlayerAim(NetworkPlayerAimData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerAim, &data);
}

void NetworkManager::RecievePlayerWeaponChange(NetworkPlayerWeaponChangeData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerWeaponChange, &data);
}

void NetworkManager::RecievePlayerHealthAndArmorChange(NetworkPlayerHealthAndArmorChangeData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerHealthAndArmorChange, &data);
}

void NetworkManager::RecievePlayerSpawnRequest(NetworkPlayerSpawnRequestData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerSpawnRequest, &data);
}

void NetworkManager::RecievePlayerModelChange(NetworkPlayerModelChangeData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerModelChange, &data);
}

void NetworkManager::RecievePlayerComponentsChange(NetworkPlayerComponentsChangeData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerComponentsChange, &data);
}

void NetworkManager::RecievePlayerChat(NetworkPlayerChatData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerChat, &data);
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

bool NetworkManager::SendTime(const unsigned char time[2])
{
	NetworkTimeData data;
	memcpy(data.time, time, sizeof(unsigned char) * 2);

	this->SendDataToAll("&NetworkManager::RecieveGameTime", &data);
	return true;
}

bool NetworkManager::SendPlayerPosition(const short index, const float pos[3], const float angle)
{
	NetworkPlayerPositionData data;
	data.client = index;
	memcpy(data.pos, pos, 3 * sizeof(float));
	data.angle = angle;

	SendDataToAll("&NetworkManager::RecievePlayerPosition", &data);
	return true;
}

bool NetworkManager::SendPlayerModel(const short index, const unsigned int model)
{
	NetworkPlayerModelChangeData data;
	data.client = index;
	data.model = model;

	SendDataToAll("&NetworkManager::RecievePlayerModelChange", &data);
	return true;
}

template <typename DATATYPE>
void NetworkManager::WriteToRPCBuffer(const NetworkManager::NetworkRPCType type, const DATATYPE *data)
{
	if (rpcbuffersize == maxrpcbuffersize)
	{
		return;
	}
#ifdef WIN32
	EnterCriticalSection(&rpcbuffersection);
#endif
	if (!ResizeBuffer<NetworkRPCData *>(rpcbuffer, rpcbuffersize + 1))
	{
		return;
	}
	rpcbuffer[rpcbuffersize].type = type;
	switch (type)
	{
	case NetworkRPCPlayerConnectionRequest:
		{
			rpcbuffer[rpcbuffersize].data.playerconnectionrequest = (NetworkPlayerConnectionRequestDataInternal *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerconnectionrequest, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerConnectionConfirmation:
		{
			rpcbuffer[rpcbuffersize].data.playerconnectionconfirmation = (NetworkPlayerConnectionConfirmationData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerconnectionconfirmation, data, sizeof(DATATYPE));
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
	case NetworkRPCPlayerSpawnRequest:
		{
			rpcbuffer[rpcbuffersize].data.playerspawnrequest = (NetworkPlayerSpawnRequestData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerspawnrequest, data, sizeof(DATATYPE));
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
	}
	rpcbuffersize++;
#ifdef WIN32
	LeaveCriticalSection(&rpcbuffersection);
#endif
}

void NetworkManager::HandleRPCData(const NetworkRPCType type, const NetworkRPCUnion *data)
{
	if (data == NULL)
	{
		return;
	}
	switch (type)
	{
	case NetworkRPCPlayerConnectionRequest:
		{
			short client = this->GetClientIndex(data->playerconnectionrequest->address);
			if (client != INVALID_PLAYER_INDEX)
			{
				PrintToServer(L"Player %s has tried to connect twice.", playm.playerbuffer[client]->name);
				this->SendConnectionError(data->playerconnectionrequest->address, NetworkPlayerConnectionErrorAlreadyConnected);
				net->CloseConnection(data->playerconnectionrequest->address, true);
				return;
			}
			client = this->RegisterNewClient(data->playerconnectionrequest->address);
			if (client == INVALID_PLAYER_INDEX)
			{
				PrintToServer(L"Unable to register new client.");
				this->SendConnectionError(data->playerconnectionrequest->address, NetworkPlayerConnectionErrorAllocationError);
				net->CloseConnection(data->playerconnectionrequest->address, true);
				delete data->playerconnectionrequest;
				return;
			}
			if (!vmm.OnPlayerConnect(client, data->playerconnectionrequest->name))
			{
				PrintToServer(L"Server script has disabled new client.");
				this->SendConnectionError(data->playerconnectionrequest->address, NetworkPlayerConnectionErrorScriptLock);
				net->CloseConnection(data->playerconnectionrequest->address, true);
				delete clientbuffer[client];
				clientbuffer[client] = NULL;
				delete data->playerconnectionrequest;
				return;
			}
			if (!playm.RegisterNewPlayer(client, data->playerconnectionrequest->name))
			{
				PrintToServer(L"Unable to register new client.");
				this->SendConnectionError(data->playerconnectionrequest->address, NetworkPlayerConnectionErrorAllocationError);
				net->CloseConnection(data->playerconnectionrequest->address, true);
				delete clientbuffer[client];
				clientbuffer[client] = NULL;
				delete data->playerconnectionrequest;
				return;
			}
			PrintToServer(L"Player %s[%d] connected", playm.playerbuffer[client]->name, client);
			NetworkPlayerInfoData infodata;
			infodata.index = client;
			infodata.sessionkey = data->playerconnectionrequest->sessionkey;
			rpc3->CallCPP("&NetworkManager::RecieveClientInfo", defaultclientid, infodata, rpc3);
			delete data->playerconnectionrequest; 
			break;
		}
	case NetworkRPCPlayerConnectionConfirmation:
		{
			NetworkTimeData timedata;
			server.GetTime(timedata.time);
			this->SendDataToOne("&NetworkManager::RecieveGameTime", data->playerconnectionconfirmation->client, &timedata);

			NetworkPlayerFullUpdateData *playerdata;
			//TODO: Optimize using currently connected players, not buffer size.
			for (short i = 0; i < clientbuffersize; i++)
			{
				if (i != data->playerconnectionconfirmation->client)
				{
					playerdata = this->GetPlayerFullUpdateData(i);
					if (playerdata != NULL)
					{
						this->SendDataToOne("&NetworkManager::RecievePlayerFullUpdate", data->playerconnectionconfirmation->client, playerdata);
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
					this->SendDataToOne("&NetworkManager::RecieveVehicleFullUpdate", data->playerconnectionconfirmation->client, vehicledata);
					delete vehicledata;
				}
			}
			playerdata = this->GetPlayerFullUpdateData(data->playerconnectionconfirmation->client);
			if (playerdata == NULL)
			{
				PrintToServer(L"Unable to read player data.");
				this->SendConnectionError(clientbuffer[data->playerconnectionconfirmation->client]->address, NetworkPlayerConnectionErrorAllocationError);
				net->CloseConnection(clientbuffer[data->playerconnectionconfirmation->client]->address, true);
				delete clientbuffer[data->playerconnectionconfirmation->client];
				clientbuffer[data->playerconnectionconfirmation->client] = NULL;
				delete data->playerconnectionconfirmation;
				return;
			}
			this->SendDataToAll("&NetworkManager::RecieveClientConnection", playerdata);
			delete playerdata;
			server.UpdateServerInfo();

			delete data->playerconnectionconfirmation;
			break;
		}
	case NetworkRPCPlayerMove:
		{
			if ((data->playermove->client < 0) || (data->playermove->client >= playm.playerbuffersize))
			{
				delete data->playermove;
				return;
			}
			if (playm.playerbuffer[data->playermove->client] == NULL)
			{
				delete data->playermove;
				return;
			}
			memcpy(playm.playerbuffer[data->playermove->client]->position, data->playermove->position, sizeof(float) * 3);
			playm.playerbuffer[data->playermove->client]->angle = data->playermove->angle;
			if (playm.playerbuffer[data->playermove->client]->car_enter == true)
			{
				delete data->playermove;
				return;
			}
			if (playm.playerbuffer[data->playermove->client]->vehicleindex != INVALID_VEHICLE_INDEX)
			{

				vm.SetVehiclePositionInternal(playm.playerbuffer[data->playermove->client]->vehicleindex, data->playermove->position);
				vm.SetVehicleAngleInternal(playm.playerbuffer[data->playermove->client]->vehicleindex, data->playermove->angle);
			}
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerMove", data->playermove->client, data->playermove);
			delete data->playermove;
			break;
		}
	case NetworkRPCPlayerJump:
		{
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerJump", data->playerjump->client, data->playerjump);
			delete data->playerjump;
			break;
		}
	case NetworkRPCPlayerDuck:
		{
			if ((data->playerduck->client < 0) || (data->playerduck->client >= playm.playerbuffersize))
			{
				delete data->playerduck;
				return;
			}
			if (playm.playerbuffer[data->playerduck->client] == NULL)
			{
				delete data->playerduck;
				return;
			}
			playm.playerbuffer[data->playerduck->client]->isducking = data->playerduck->shouldduck;
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerDuck", data->playerduck->client, data->playerduck);
			delete data->playerduck;
			break;
		}
	case NetworkRPCPlayerEntranceInVehicle:
		{
			if ((data->playerentranceinvehicle->client < 0) || (data->playerentranceinvehicle->client >= playm.playerbuffersize))
			{
				delete data->playerentranceinvehicle;
				return;
			}
			if (playm.playerbuffer[data->playerentranceinvehicle->client] == NULL)
			{
				delete data->playerentranceinvehicle;
				return;
			}
			if (data->playerentranceinvehicle->seat == INVALID_PLAYER_SEAT_INDEX)
			{
				playm.playerbuffer[data->playerentranceinvehicle->client]->car_enter = false;
				delete data->playerentranceinvehicle;
				return;
			}
			playm.playerbuffer[data->playerentranceinvehicle->client]->car_enter = true;
			playm.playerbuffer[data->playerentranceinvehicle->client]->seatindex = data->playerentranceinvehicle->seat;
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerEntranceInVehicle", data->playerentranceinvehicle->client, data->playerentranceinvehicle);
			delete data->playerentranceinvehicle;
			break;
		}
	case NetworkRPCPlayerCancelEntranceInVehicle:
		{
			if ((data->playercancelentranceinvehicle->client < 0) || (data->playercancelentranceinvehicle->client >= playm.playerbuffersize))
			{
				delete data->playercancelentranceinvehicle;
				return;
			}
			if (playm.playerbuffer[data->playercancelentranceinvehicle->client] == NULL)
			{
				delete data->playercancelentranceinvehicle;
				return;
			}
			playm.playerbuffer[data->playercancelentranceinvehicle->client]->vehicleindex = INVALID_VEHICLE_INDEX;
			playm.playerbuffer[data->playercancelentranceinvehicle->client]->car_enter = false;
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerCancelEntranceInVehicle", data->playercancelentranceinvehicle->client, data->playercancelentranceinvehicle);
			delete data->playercancelentranceinvehicle;
			break;
		}
	case NetworkRPCPlayerExitFromVehicle:
		{
			if ((data->playerexitfromvehicle->client < 0) || (data->playerexitfromvehicle->client >= playm.playerbuffersize))
			{
				delete data->playerexitfromvehicle;
				return;
			}
			if (playm.playerbuffer[data->playerexitfromvehicle->client] == NULL)
			{
				delete data->playerexitfromvehicle;
				return;
			}
			playm.playerbuffer[data->playerexitfromvehicle->client]->vehicleindex = INVALID_VEHICLE_INDEX;
			playm.playerbuffer[data->playerexitfromvehicle->client]->car_enter = false;
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerExitFromVehicle", data->playerexitfromvehicle->client, data->playerexitfromvehicle);
			delete data->playerexitfromvehicle;
			break;
		}
	case NetworkRPCPlayerFire:
		{
			if ((data->playerfire->client < 0) || (data->playerfire->client >= playm.playerbuffersize))
			{
				delete data->playerfire;
				return;
			}
			if (playm.playerbuffer[data->playerfire->client] == NULL)
			{
				delete data->playerfire;
				return;
			}
			playm.playerbuffer[data->playerfire->client]->currentweapon = data->playerfire->weapon;
			if (data->playerfire->target != INVALID_PLAYER_INDEX)
			{
				if (data->playerfire->target >= playm.playerbuffersize)
				{
					delete data->playerfire;
					return;
				}
				if (playm.playerbuffer[data->playerfire->target] == NULL)
				{
					delete data->playerfire;
					return;
				}
				playm.playerbuffer[data->playerfire->target]->health = data->playerfire->health;
				playm.playerbuffer[data->playerfire->target]->armor = data->playerfire->armor;
			}
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerFire", data->playerfire->client, data->playerfire);
			delete data->playerfire;
			break;
		}
	case NetworkRPCPlayerAim:
		{
			if ((data->playeraim->client < 0) || (data->playeraim->client >= playm.playerbuffersize))
			{
				delete data->playeraim;
				return;
			}
			if (playm.playerbuffer[data->playeraim->client] == NULL)
			{
				delete data->playeraim;
				return;
			}
			playm.playerbuffer[data->playeraim->client]->currentweapon = data->playeraim->weapon;
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerAim", data->playeraim->client, data->playeraim);
			delete data->playeraim;
			break;
		}
	case NetworkRPCPlayerWeaponChange:
		{
			if ((data->playerweaponchange->client < 0) || (data->playerweaponchange->client >= playm.playerbuffersize))
			{
				delete data->playerweaponchange;
				return;
			}
			if (playm.playerbuffer[data->playerweaponchange->client] == NULL)
			{
				delete data->playerweaponchange;
				return;
			}
			playm.playerbuffer[data->playerweaponchange->client]->currentweapon = data->playerweaponchange->weapon;
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerWeaponChange", data->playerweaponchange->client, data->playerweaponchange);
			delete data->playerweaponchange;
			break;
		}
	case NetworkRPCPlayerHealthAndArmorChange:
		{
			if ((data->playerhealthandarmorchange->client < 0) || (data->playerhealthandarmorchange->client >= playm.playerbuffersize))
			{
				delete data->playerhealthandarmorchange;
				return;
			}
			if (playm.playerbuffer[data->playerhealthandarmorchange->client] == NULL)
			{
				delete data->playerhealthandarmorchange;
				return;
			}
			playm.playerbuffer[data->playerhealthandarmorchange->client]->health = data->playerhealthandarmorchange->health;
			playm.playerbuffer[data->playerhealthandarmorchange->client]->armor = data->playerhealthandarmorchange->armor;
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerHealthAndArmorChange", data->playerhealthandarmorchange->client, data->playerhealthandarmorchange);
			delete data->playerhealthandarmorchange;
			break;
		}
	case NetworkRPCPlayerSpawnRequest:
		{
			if ((data->playerspawnrequest->client < 0) || (data->playerspawnrequest->client >= playm.playerbuffersize))
			{
				delete data->playerspawnrequest;
				return;
			}
			if (playm.playerbuffer[data->playerspawnrequest->client] == NULL)
			{
				delete data->playerspawnrequest;
				return;
			}
			NetworkPlayerSpawnData data2;

			playm.playerbuffer[data->playerspawnrequest->client]->armor = 0;
			playm.playerbuffer[data->playerspawnrequest->client]->health = 200;
			playm.playerbuffer[data->playerspawnrequest->client]->room = 0;
			playm.playerbuffer[data->playerspawnrequest->client]->want_spawn = 1;

			PrintToServer(L"Spawn RPC %d", data->playerspawnrequest->client);
			vmm.OnPlayerSpawn(data->playerspawnrequest->client);

			playm.playerbuffer[data->playerspawnrequest->client]->want_spawn = 0;
			data2.armor = playm.playerbuffer[data->playerspawnrequest->client]->armor;
			data2.health = playm.playerbuffer[data->playerspawnrequest->client]->health;
			data2.model = playm.playerbuffer[data->playerspawnrequest->client]->model;
			data2.angle = playm.playerbuffer[data->playerspawnrequest->client]->spawn_pos[3];
			data2.room = playm.playerbuffer[data->playerspawnrequest->client]->room;
			memcpy(data2.position, playm.playerbuffer[data->playerspawnrequest->client]->spawn_pos, sizeof(float) * 3);
			memcpy(data2.compD, playm.playerbuffer[data->playerspawnrequest->client]->compD, sizeof(int) * 11);
			memcpy(data2.compT, playm.playerbuffer[data->playerspawnrequest->client]->compT, sizeof(int) * 11);
			data2.client = data->playerspawnrequest->client;

			this->SendDataToAll("&NetworkManager::RecievePlayerSpawn", &data2);

			NetworkTimeData timedata;
			server.GetTime(timedata.time);
			this->SendDataToOne("&NetworkManager::RecieveGameTime", data->playerspawnrequest->client, &timedata);

			delete data->playerspawnrequest;
			break;
		}
	case NetworkRPCPlayerModelChange:
		{
			if ((data->playermodelchange->client < 0) || (data->playermodelchange->client >= playm.playerbuffersize))
			{
				delete data->playermodelchange;
				return;
			}
			if (playm.playerbuffer[data->playermodelchange->client] == NULL)
			{
				delete data->playermodelchange;
				return;
			}
			playm.playerbuffer[data->playermodelchange->client]->model = data->playermodelchange->model;
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerModelChange", data->playermodelchange->client, data->playermodelchange);
			delete data->playermodelchange;
			break;
		}
	case NetworkRPCPlayerComponentsChange:
		{
			if ((data->playercomponentschange->client < 0) || (data->playercomponentschange->client >= playm.playerbuffersize))
			{
				delete data->playercomponentschange;
				return;
			}
			if (playm.playerbuffer[data->playercomponentschange->client] == NULL)
			{
				delete data->playercomponentschange;
				return;
			}
			memcpy(playm.playerbuffer[data->playercomponentschange->client]->compD, data->playercomponentschange->compD, sizeof(int) * 11);
			memcpy(playm.playerbuffer[data->playercomponentschange->client]->compT, data->playercomponentschange->compT, sizeof(int) * 11);
			this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerComponentsChange", data->playercomponentschange->client, data->playercomponentschange);
			delete data->playercomponentschange;
			break;
		}
	case NetworkRPCPlayerChat:
		{
			if (!vmm.OnPlayerText(data->playerchat->client, data->playerchat->message))
			{
				delete data->playerchat;
				return;
			}
			this->SendDataToAll("&NetworkManager::RecievePlayerChat", data->playerchat);
			delete data->playerchat;
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
		case NetworkRPCPlayerConnectionRequest:
			{
				delete rpcbuffer[i].data.playerconnectionrequest;
				break;
			}
		case NetworkRPCPlayerConnectionConfirmation:
			{
				delete rpcbuffer[i].data.playerconnectionconfirmation;
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
		case NetworkRPCPlayerSpawnRequest:
			{
				delete rpcbuffer[i].data.playerspawnrequest;
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
		}
	}
	free(rpcbuffer);
	rpcbuffer = NULL;
	rpcbuffersize = 0;
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
		if (!ResizeBuffer<Client **>(clientbuffer, index + 1))
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

void NetworkManager::HandleClientDisconnection(const SystemAddress address)
{
	short client = this->GetClientIndex(address);
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
	vmm.OnPlayerDisconnect(client);
	PrintToServer(L"Player %s disconnected", playm.playerbuffer[client]->name);
	delete playm.playerbuffer[client];
	playm.playerbuffer[client] = NULL;
	playm.numplayers--;
	if ((client < 0) && (client >= clientbuffersize))
	{
		return;
	}
	if (clientbuffer[client] == NULL)
	{
		return;
	}
	delete clientbuffer[client];
	clientbuffer[client] = NULL;
	NetworkPlayerDisconnectionData data;
	data.client = client;
	this->SendDataToAll("&NetworkManager::RecieveClientDisconnection", &data);
	server.UpdateServerInfo();
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
	wcscpy(data->name, playm.playerbuffer[index]->name);
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
void NetworkManager::SendDataToOne(const char *RPC, const short index, const DATATYPE *data)
{
	if((index < 0) && (index >= clientbuffersize))
	{
		return;
	}
	if (clientbuffer[index] == NULL)
	{
		return;
	}
	rpc3->CallCPP(RPC, clientbuffer[index]->id, *data, rpc3);
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