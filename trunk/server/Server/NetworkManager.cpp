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
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerStartEntranceInVehicle);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerCancelEntranceInVehicle);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerFinishEntranceInVehicle);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerStartExitFromVehicle);
	RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::RecievePlayerFinishExitFromVehicle);
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
			this->SendConnectionError(clientrpc3->GetLastSenderAddress(), NetworkPlayerConnectionErrorInvalidAuth);
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

void NetworkManager::RecievePlayerStartEntranceInVehicle(NetworkPlayerStartEntranceInVehicleData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerStartEntranceInVehicle, &data);
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

void NetworkManager::RecievePlayerFinishEntranceInVehicle(NetworkPlayerFinishEntranceInVehicleData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerFinishEntranceInVehicle, &data);
}

void NetworkManager::RecievePlayerStartExitFromVehicle(NetworkPlayerStartExitFromVehicleData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerStartExitFromVehicle, &data);
}

void NetworkManager::RecievePlayerFinishExitFromVehicle(NetworkPlayerFinishExitFromVehicleData data, RakNet::RPC3 *clientrpc3)
{
	short client = this->GetClientIndex(clientrpc3->GetLastSenderAddress());
	if (client == INVALID_PLAYER_INDEX)
	{
		return;
	}
	data.client = client;
	this->WriteToRPCBuffer(NetworkRPCPlayerFinishExitFromVehicle, &data);
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

bool NetworkManager::SendGameTimeChangeToAll(const unsigned char time[2])
{
	NetworkGameTimeChangeData data;
	memcpy(data.time, time, sizeof(unsigned char) * 2);
	this->SendDataToAll("&NetworkManager::RecieveGameTimeChange", &data);
	return true;
}

bool NetworkManager::SendPlayerModelChangeToAll(const short index)
{
	if ((index < 0) || (index >= playm.playerbuffersize))
	{
		return false;
	}
	if (playm.playerbuffer[index] == NULL)
	{
		return false;
	}
	NetworkPlayerModelChangeData data;
	data.client = index;
	data.model = playm.playerbuffer[index]->model;
	SendDataToAll("&NetworkManager::RecievePlayerModelChange", &data);
	return true;
}

bool NetworkManager::SendPlayerWeaponGiftToAll(const short index, const eWeaponSlot slot)
{
	if ((index < 0) || (index >= playm.playerbuffersize))
	{
		return false;
	}
	if (playm.playerbuffer[index] == NULL)
	{
		return false;
	}
	if (playm.playerbuffer[index]->ammo[slot] == 0)
	{
		return false;
	}
	NetworkPlayerWeaponGiftData data;
	data.client = index;
	data.weapon = playm.playerbuffer[index]->weapons[slot];
	data.ammo = playm.playerbuffer[index]->ammo[slot];
	SendDataToAll("&NetworkManager::RecievePlayerWeaponGift", &data);
	return true;
}

bool NetworkManager::SendPlayerSpawnPositionChange(const short index)
{
	if ((index < 0) || (index >= playm.playerbuffersize))
	{
		return false;
	}
	if (playm.playerbuffer[index] == NULL)
	{
		return false;
	}
	NetworkPlayerSpawnPositionChangeData data;
	memcpy(data.position, playm.playerbuffer[index]->spawnposition, sizeof(float) * 4);
	return SendDataToOne("&NetworkManager::RecievePlayerSpawnPositionChange", index, &data);
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

bool NetworkManager::SendPlayerPosition(const short index, const float pos[3], const float angle)
{
	//TODO: Redo.
	NetworkPlayerPositionData data;
	data.client = index;
	memcpy(data.pos, pos, 3 * sizeof(float));
	data.angle = angle;

	SendDataToAll("&NetworkManager::RecievePlayerPosition", &data);
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
	case NetworkRPCPlayerStartEntranceInVehicle:
		{
			rpcbuffer[rpcbuffersize].data.playerstartentranceinvehicle = (NetworkPlayerStartEntranceInVehicleData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerstartentranceinvehicle, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerCancelEntranceInVehicle:
		{
			rpcbuffer[rpcbuffersize].data.playercancelentranceinvehicle = (NetworkPlayerCancelEntranceInVehicleData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playercancelentranceinvehicle, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerFinishEntranceInVehicle:
		{
			rpcbuffer[rpcbuffersize].data.playerfinishentranceinvehicle = (NetworkPlayerFinishEntranceInVehicleData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerfinishentranceinvehicle, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerStartExitFromVehicle:
		{
			rpcbuffer[rpcbuffersize].data.playerstartexitfromvehicle = (NetworkPlayerStartExitFromVehicleData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerstartexitfromvehicle, data, sizeof(DATATYPE));
			break;
		}
	case NetworkRPCPlayerFinishExitFromVehicle:
		{
			rpcbuffer[rpcbuffersize].data.playerfinishexitfromvehicle = (NetworkPlayerFinishExitFromVehicleData *)new DATATYPE;
			memcpy(rpcbuffer[rpcbuffersize].data.playerfinishexitfromvehicle, data, sizeof(DATATYPE));
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
			this->HandleClientConnectionRequest(data->playerconnectionrequest);
			delete data->playerconnectionrequest;
			break;
		}
	case NetworkRPCPlayerConnectionConfirmation:
		{
			this->HandleClientConnectionConfirmation(data->playerconnectionconfirmation);
			delete data->playerconnectionconfirmation;
			break;
		}
	case NetworkRPCPlayerMove:
		{
			this->HandlePlayerMove(data->playermove);
			delete data->playermove;
			break;
		}
	case NetworkRPCPlayerJump:
		{
			this->HandlePlayerJump(data->playerjump);
			delete data->playerjump;
			break;
		}
	case NetworkRPCPlayerDuck:
		{
			this->HandlePlayerDuck(data->playerduck);
			delete data->playerduck;
			break;
		}
	case NetworkRPCPlayerStartEntranceInVehicle:
		{
			this->HandlePlayerStartEntranceInVehicle(data->playerstartentranceinvehicle);
			delete data->playerstartentranceinvehicle;
			break;
		}
	case NetworkRPCPlayerCancelEntranceInVehicle:
		{
			this->HandlePlayerCancelEntranceInVehicle(data->playercancelentranceinvehicle);
			delete data->playercancelentranceinvehicle;
			break;
		}
	case NetworkRPCPlayerFinishEntranceInVehicle:
		{
			this->HandlePlayerFinishEntranceInVehicle(data->playerfinishentranceinvehicle);
			delete data->playerfinishentranceinvehicle;
			break;
		}
	case NetworkRPCPlayerStartExitFromVehicle:
		{
			this->HandlePlayerStartExitFromVehicle(data->playerstartexitfromvehicle);
			delete data->playerstartexitfromvehicle;
			break;
		}
	case NetworkRPCPlayerFinishExitFromVehicle:
		{
			this->HandlePlayerFinishExitFromVehicle(data->playerfinishexitfromvehicle);
			delete data->playerfinishexitfromvehicle;
			break;
		}
	case NetworkRPCPlayerFire:
		{
			this->HandlePlayerFire(data->playerfire);
			delete data->playerfire;
			break;
		}
	case NetworkRPCPlayerAim:
		{
			this->HandlePlayerAim(data->playeraim);
			delete data->playeraim;
			break;
		}
	case NetworkRPCPlayerWeaponChange:
		{
			this->HandlePlayerWeaponChange(data->playerweaponchange);
			delete data->playerweaponchange;
			break;
		}
	case NetworkRPCPlayerHealthAndArmorChange:
		{
			this->HandlePlayerHealthAndArmorChange(data->playerhealthandarmorchange);
			delete data->playerhealthandarmorchange;
			break;
		}
	case NetworkRPCPlayerSpawnRequest:
		{
			this->HandlePlayerSpawnRequest(data->playerspawnrequest);
			delete data->playerspawnrequest;
			break;
		}
	case NetworkRPCPlayerModelChange:
		{
			this->HandlePlayerModelChange(data->playermodelchange);
			delete data->playermodelchange;
			break;
		}
	case NetworkRPCPlayerComponentsChange:
		{
			this->HandlePlayerComponentsChange(data->playercomponentschange);
			delete data->playercomponentschange;
			break;
		}
	case NetworkRPCPlayerChat:
		{
			this->HandlePlayerChat(data->playerchat);
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
		case NetworkRPCPlayerStartEntranceInVehicle:
			{
				delete rpcbuffer[i].data.playerstartentranceinvehicle;
				break;
			}
		case NetworkRPCPlayerCancelEntranceInVehicle:
			{
				delete rpcbuffer[i].data.playercancelentranceinvehicle;
				break;
			}
		case NetworkRPCPlayerFinishEntranceInVehicle:
			{
				delete rpcbuffer[i].data.playerfinishentranceinvehicle;
				break;
			}
		case NetworkRPCPlayerStartExitFromVehicle:
			{
				delete rpcbuffer[i].data.playerstartexitfromvehicle;
				break;
			}
		case NetworkRPCPlayerFinishExitFromVehicle:
			{
				delete rpcbuffer[i].data.playerfinishexitfromvehicle;
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

void NetworkManager::HandleClientConnectionRequest(NetworkManager::NetworkPlayerConnectionRequestDataInternal *data)
{
	short client = this->GetClientIndex(data->address);
	if (client != INVALID_PLAYER_INDEX)
	{
		PrintToServer(L"Player %s has tried to connect twice.", playm.playerbuffer[client]->name);
		this->SendConnectionError(data->address, NetworkPlayerConnectionErrorAlreadyConnected);
		net->CloseConnection(data->address, true);
		return;
	}
	client = this->RegisterNewClient(data->address);
	if (client == INVALID_PLAYER_INDEX)
	{
		PrintToServer(L"Unable to register new client.");
		this->SendConnectionError(data->address, NetworkPlayerConnectionErrorAllocationError);
		net->CloseConnection(data->address, true);
		return;
	}
	if (!vmm.OnPlayerConnect(client, data->name))
	{
		PrintToServer(L"Server script has disabled new client.");
		this->SendConnectionError(data->address, NetworkPlayerConnectionErrorScriptLock);
		net->CloseConnection(data->address, true);
		delete clientbuffer[client];
		clientbuffer[client] = NULL;
		return;
	}
	if (!playm.RegisterNewPlayer(client, data->name))
	{
		PrintToServer(L"Unable to register new client.");
		this->SendConnectionError(data->address, NetworkPlayerConnectionErrorAllocationError);
		net->CloseConnection(data->address, true);
		delete clientbuffer[client];
		clientbuffer[client] = NULL;
		return;
	}
	PrintToServer(L"Player %s[%d] connected", playm.playerbuffer[client]->name, client);
	NetworkPlayerInfoData infodata;
	infodata.index = client;
	infodata.sessionkey = data->sessionkey;
	rpc3->SetRecipientAddress(data->address, false);
	rpc3->SetRecipientObject(defaultclientid);
	rpc3->Call("&NetworkManager::RecieveClientInfo", infodata, rpc3);
	rpc3->SetRecipientAddress(UNASSIGNED_SYSTEM_ADDRESS, true);
	rpc3->SetRecipientObject(UNASSIGNED_NETWORK_ID); 
}

void NetworkManager::HandleClientConnectionConfirmation(const NetworkPlayerConnectionConfirmationData *data)
{
	NetworkGameTimeChangeData timedata;
	server.GetGameTime(timedata.time);
	this->SendDataToOne("&NetworkManager::RecieveGameTimeChange", data->client, &timedata);
	NetworkPlayerFullUpdateData *playerdata;
	//TODO: Optimize using currently connected players, not buffer size.
	for (short i = 0; i < clientbuffersize; i++)
	{
		if (i != data->client)
		{
			playerdata = this->GetPlayerFullUpdateData(i);
			if (playerdata != NULL)
			{
				this->SendDataToOne("&NetworkManager::RecievePlayerFullUpdate", data->client, playerdata);
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
			this->SendDataToOne("&NetworkManager::RecieveVehicleFullUpdate", data->client, vehicledata);
			delete vehicledata;
		}
	}
	playerdata = this->GetPlayerFullUpdateData(data->client);
	if (playerdata == NULL)
	{
		PrintToServer(L"Unable to read player data.");
		this->SendConnectionError(clientbuffer[data->client]->address, NetworkPlayerConnectionErrorAllocationError);
		net->CloseConnection(clientbuffer[data->client]->address, true);
		delete clientbuffer[data->client];
		clientbuffer[data->client] = NULL;
		return;
	}
	this->SendDataToAll("&NetworkManager::RecieveClientConnection", playerdata);
	delete playerdata;
	server.UpdateServerInfo();
}

void NetworkManager::HandlePlayerMove(const NetworkPlayerMoveData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	memcpy(playm.playerbuffer[data->client]->position, data->position, sizeof(float) * 3);
	playm.playerbuffer[data->client]->angle = data->angle;
	if (playm.playerbuffer[data->client]->state == PlayerStateEnteringVehicle)
	{
		return;
	}
	if (playm.playerbuffer[data->client]->vehicleindex != INVALID_VEHICLE_INDEX)
	{
		vm.SetVehiclePositionInternal(playm.playerbuffer[data->client]->vehicleindex, data->position);
		vm.SetVehicleAngleInternal(playm.playerbuffer[data->client]->vehicleindex, data->angle);
	}
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerMove", data->client, data);
}

void NetworkManager::HandlePlayerJump(const NetworkPlayerJumpData *data)
{
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerJump", data->client, data);
}

void NetworkManager::HandlePlayerDuck(const NetworkPlayerDuckData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	playm.playerbuffer[data->client]->isducking = data->shouldduck;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerDuck", data->client, data);
}

void NetworkManager::HandlePlayerStartEntranceInVehicle(const NetworkPlayerStartEntranceInVehicleData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	if (playm.playerbuffer[data->client]->state != PlayerStateOnFoot)
	{
		return;
	}
	if ((data->vehicleindex < 0) || (data->vehicleindex >= vm.vehiclebuffersize))
	{
		return;
	}
	if (vm.vehiclebuffer[data->vehicleindex] == NULL)
	{
		return;
	}
	if (data->seatindex < 0)
	{
		return;
	}
	playm.playerbuffer[data->client]->state = PlayerStateEnteringVehicle;
	playm.playerbuffer[data->client]->vehicleindex = data->vehicleindex;
	playm.playerbuffer[data->client]->seatindex = data->seatindex;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerStartEntranceInVehicle", data->client, data);
}

void NetworkManager::HandlePlayerCancelEntranceInVehicle(const NetworkPlayerCancelEntranceInVehicleData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	if (playm.playerbuffer[data->client]->state != PlayerStateEnteringVehicle)
	{
		return;
	}
	playm.playerbuffer[data->client]->state = PlayerStateOnFoot;
	playm.playerbuffer[data->client]->vehicleindex = INVALID_VEHICLE_INDEX;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerCancelEntranceInVehicle", data->client, data);
}

void NetworkManager::HandlePlayerFinishEntranceInVehicle(const NetworkPlayerFinishEntranceInVehicleData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	if (playm.playerbuffer[data->client]->state != PlayerStateEnteringVehicle)
	{
		return;
	}
	playm.playerbuffer[data->client]->state = PlayerStateInVehicle;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerFinishEntranceInVehicle", data->client, data);
}

void NetworkManager::HandlePlayerStartExitFromVehicle(const NetworkPlayerStartExitFromVehicleData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	if (playm.playerbuffer[data->client]->state != PlayerStateInVehicle)
	{
		return;
	}
	playm.playerbuffer[data->client]->state = PlayerStateExitingVehicle;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerStartExitFromVehicle", data->client, data);
}

void NetworkManager::HandlePlayerFinishExitFromVehicle(const NetworkPlayerFinishExitFromVehicleData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	if (playm.playerbuffer[data->client]->state != PlayerStateExitingVehicle)
	{
		return;
	}
	playm.playerbuffer[data->client]->state = PlayerStateOnFoot;
	playm.playerbuffer[data->client]->vehicleindex = INVALID_VEHICLE_INDEX;
	playm.playerbuffer[data->client]->seatindex = INVALID_VEHICLE_INDEX;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerFinishExitFromVehicle", data->client, data);
}

void NetworkManager::HandlePlayerFire(const NetworkPlayerFireData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	playm.playerbuffer[data->client]->currentweapon = data->weapon;
	if (data->target != INVALID_PLAYER_INDEX)
	{
		if ((data->target < INVALID_PLAYER_INDEX) && (data->target >= playm.playerbuffersize))
		{
			return;
		}
		if (playm.playerbuffer[data->target] == NULL)
		{
			return;
		}
		playm.playerbuffer[data->target]->health = data->health;
		playm.playerbuffer[data->target]->armor = data->armor;
		if (playm.playerbuffer[data->target]->health < 100)
		{
			vmm.OnPlayerDeath(data->target, data->client, data->weapon);
		}
	}
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerFire", data->client, data);
}

void NetworkManager::HandlePlayerAim(const NetworkPlayerAimData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	playm.playerbuffer[data->client]->currentweapon = data->weapon;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerAim", data->client, data);
}

void NetworkManager::HandlePlayerWeaponChange(const NetworkPlayerWeaponChangeData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	playm.playerbuffer[data->client]->currentweapon = data->weapon;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerWeaponChange", data->client, data);
}

void NetworkManager::HandlePlayerHealthAndArmorChange(const NetworkPlayerHealthAndArmorChangeData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	playm.playerbuffer[data->client]->health = data->health;
	playm.playerbuffer[data->client]->armor = data->armor;
	if (playm.playerbuffer[data->client]->health < 100)
	{
		vmm.OnPlayerDeath(data->client, INVALID_PLAYER_INDEX, 46);
	}
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerHealthAndArmorChange", data->client, data);
}

void NetworkManager::HandlePlayerSpawnRequest(const NetworkPlayerSpawnRequestData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	NetworkPlayerSpawnData data2;
	playm.playerbuffer[data->client]->armor = 0;
	playm.playerbuffer[data->client]->health = 200;
	playm.playerbuffer[data->client]->room = 0;
	vmm.OnPlayerSpawn(data->client);
	data2.armor = playm.playerbuffer[data->client]->armor;
	data2.health = playm.playerbuffer[data->client]->health;
	data2.model = playm.playerbuffer[data->client]->model;
	memcpy(data2.position, playm.playerbuffer[data->client]->spawnposition, sizeof(float) * 4);
	data2.room = playm.playerbuffer[data->client]->room;
	memcpy(data2.compD, playm.playerbuffer[data->client]->compD, sizeof(int) * 11);
	memcpy(data2.compT, playm.playerbuffer[data->client]->compT, sizeof(int) * 11);
	data2.client = data->client;
	this->SendDataToAll("&NetworkManager::RecievePlayerSpawn", &data2);
	NetworkGameTimeChangeData timedata;
	server.GetGameTime(timedata.time);
	this->SendDataToOne("&NetworkManager::RecieveGameTimeChange", data->client, &timedata);
}

void NetworkManager::HandlePlayerModelChange(const NetworkPlayerModelChangeData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	playm.playerbuffer[data->client]->model = data->model;
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerModelChange", data->client, data);
}

void NetworkManager::HandlePlayerComponentsChange(const NetworkPlayerComponentsChangeData *data)
{
	if ((data->client < 0) || (data->client >= playm.playerbuffersize))
	{
		return;
	}
	if (playm.playerbuffer[data->client] == NULL)
	{
		return;
	}
	memcpy(playm.playerbuffer[data->client]->compD, data->compD, sizeof(int) * 11);
	memcpy(playm.playerbuffer[data->client]->compT, data->compT, sizeof(int) * 11);
	this->SendDataToAllExceptOne("&NetworkManager::RecievePlayerComponentsChange", data->client, data);
}

void NetworkManager::HandlePlayerChat(NetworkPlayerChatData *data)
{
	if (!vmm.OnPlayerText(data->client, data->message))
	{
		return;
	}
	wchar_t tempstring[MAX_CHAT_MESSAGE_LENGTH];
	_snwprintf(tempstring, MAX_CHAT_MESSAGE_LENGTH, L"%s: %s", playm.playerbuffer[data->client]->name, data->message);
	tempstring[MAX_CHAT_MESSAGE_LENGTH-1] = L'\0';
	PrintToServer(tempstring);
	wcscpy(data->message, tempstring);
	this->SendDataToAll("&NetworkManager::RecievePlayerChat", data);
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
	rpc3->SetRecipientAddress(address, false);
	rpc3->SetRecipientObject(defaultclientid);
	rpc3->Call("&NetworkManager::RecieveClientConnectionError", data, rpc3);
	rpc3->SetRecipientAddress(UNASSIGNED_SYSTEM_ADDRESS, true);
	rpc3->SetRecipientObject(UNASSIGNED_NETWORK_ID);
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
			rpc3->SetRecipientAddress(clientbuffer[i]->address, false);
			rpc3->SetRecipientObject(clientbuffer[i]->id);
			rpc3->Call(RPC, *data, rpc3);
		}
	}
	rpc3->SetRecipientAddress(UNASSIGNED_SYSTEM_ADDRESS, true);
	rpc3->SetRecipientObject(UNASSIGNED_NETWORK_ID);
}

template <typename DATATYPE>
bool NetworkManager::SendDataToOne(const char *RPC, const short index, const DATATYPE *data)
{
	if((index < 0) && (index >= clientbuffersize))
	{
		return false;
	}
	if (clientbuffer[index] == NULL)
	{
		return false;
	}
	rpc3->SetRecipientAddress(clientbuffer[index]->address, false);
	rpc3->SetRecipientObject(clientbuffer[index]->id);
	rpc3->Call(RPC, *data, rpc3);
	rpc3->SetRecipientAddress(UNASSIGNED_SYSTEM_ADDRESS, true);
	rpc3->SetRecipientObject(UNASSIGNED_NETWORK_ID);
	return true;
}

template <typename DATATYPE>
void NetworkManager::SendDataToAllExceptOne(const char *RPC, const short index, const DATATYPE *data)
{
	//TODO: Optimize using currently connected players, not buffer size.
	for (unsigned char i = 0; i < clientbuffersize; i++)
	{
		if ((i != index) && (clientbuffer[i] != NULL))
		{
			rpc3->SetRecipientAddress(clientbuffer[i]->address, false);
			rpc3->SetRecipientObject(clientbuffer[i]->id);
			rpc3->Call(RPC, *data, rpc3);
		}
	}
	rpc3->SetRecipientAddress(UNASSIGNED_SYSTEM_ADDRESS, true);
	rpc3->SetRecipientObject(UNASSIGNED_NETWORK_ID);
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