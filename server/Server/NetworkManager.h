/// \file
/// \brief Header file that describes the NetworkManager class.
/// \details See class description.
/// \author WNeZRoS, FaTony

#pragma once

#include "NetworkIDObject.h"

#include "../../Shared/Network/NetworkProtocol.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace RakNet
{
	class RPC3;
}

/// \brief A network manager. It handles all network traffic.
/// \details TODO:
/// \author WNeZRoS, FaTony

class NetworkManager : public NetworkIDObject
{
public:
	NetworkManager(void);
	~NetworkManager(void);
	void Load(short maxclients, unsigned short port);
	void Tick(void);
	void Unload(void);
	void CheckClients(void);
	void UpdateServerInfo(void);

	void RecieveClientConnectionRequest(NetworkPlayerConnectionRequestData data, RakNet::RPC3 *clientrpc3);
	void RecieveClientConnectionNextRequest(NetworkPlayerConnectionRequestData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerMove(NetworkPlayerMoveData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerJump(NetworkPlayerJumpData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerDuck(NetworkPlayerDuckData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerEntranceInVehicle(NetworkPlayerEntranceInVehicleData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerCancelEntranceInVehicle(NetworkPlayerCancelEntranceInVehicleData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerExitFromVehicle(NetworkPlayerExitFromVehicleData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerFire(NetworkPlayerFireData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerAim(NetworkPlayerAimData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerWeaponChange(NetworkPlayerWeaponChangeData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerHealthAndArmorChange(NetworkPlayerHealthAndArmorChangeData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerSpawnRequest(NetworkPlayerSpawnRequestData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerModelChange(NetworkPlayerModelChangeData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerComponentsChange(NetworkPlayerComponentsChangeData data, RakNet::RPC3 *clientrpc3);
	void RecievePlayerChat(NetworkPlayerChatData data, RakNet::RPC3 *clientrpc3);

	bool SendNewVehicleInfoToAll(const short index);
	bool SendTime(const int h, const int m);
	bool SendPlayerPosition(const short index, const float pos[3], const float angle);
	bool SendPlayerModel(const short index, const unsigned int model);
private:
	unsigned short serverport;
	NetworkIDManager *manager;
	NetworkID serverid;
	NetworkID defaultclientid;
	RakNet::RPC3 *rpc3;
	RakPeerInterface *net;
	struct Client
	{
		SystemAddress address;
		NetworkID id;
	};
	short maxclientbuffersize;
	short clientbuffersize;
	Client **clientbuffer;
	enum NetworkRPCType
	{
		NetworkRPCPlayerConnectionRequest,
		NetworkRPCPlayerConnectionNextRequest,
		NetworkRPCPlayerMove,
		NetworkRPCPlayerJump,
		NetworkRPCPlayerDuck,
		NetworkRPCPlayerEntranceInVehicle,
		NetworkRPCPlayerCancelEntranceInVehicle,
		NetworkRPCPlayerExitFromVehicle,
		NetworkRPCPlayerFire,
		NetworkRPCPlayerAim,
		NetworkRPCPlayerWeaponChange,
		NetworkRPCPlayerHealthAndArmorChange,
		NetworkRPCPlayerSpawnRequest,
		NetworkRPCPlayerModelChange,
		NetworkRPCPlayerComponentsChange,
		NetworkRPCPlayerChat
	};
	struct NetworkPlayerConnectionRequestDataInternal
	{
		SystemAddress address;
		wchar_t name[MAX_PLAYER_NAME_LENGTH];
		char sessionkey[33];
	};
	union NetworkRPCUnion
	{
		NetworkPlayerConnectionRequestDataInternal *playerconnectionrequest;
		NetworkPlayerConnectionRequestDataInternal *playerconnectionnextrequest;
		NetworkPlayerMoveData *playermove;
		NetworkPlayerJumpData *playerjump;
		NetworkPlayerDuckData *playerduck;
		NetworkPlayerEntranceInVehicleData *playerentranceinvehicle;
		NetworkPlayerCancelEntranceInVehicleData *playercancelentranceinvehicle;
		NetworkPlayerExitFromVehicleData *playerexitfromvehicle;
		NetworkPlayerFireData *playerfire;
		NetworkPlayerAimData *playeraim;
		NetworkPlayerWeaponChangeData *playerweaponchange;
		NetworkPlayerHealthAndArmorChangeData *playerhealthandarmorchange;
		NetworkPlayerSpawnRequestData *playerspawnrequest;
		NetworkPlayerModelChangeData *playermodelchange;
		NetworkPlayerComponentsChangeData *playercomponentschange;
		NetworkPlayerChatData *playerchat;
	};
	struct NetworkRPCData
	{
		NetworkRPCType type;
		NetworkRPCUnion data;
	};
	int maxrpcbuffersize;
	int rpcbuffersize;
	NetworkRPCData *rpcbuffer;
#ifdef WIN32
	CRITICAL_SECTION rpcbuffersection;
#endif
	template <typename DATATYPE>
	void WriteToRPCBuffer(const NetworkRPCType type, const DATATYPE *data);
	void HandleRPCData(const NetworkRPCType type, const NetworkRPCUnion *data);
	void FreeRPCBuffer(void);
	short RegisterNewClient(const SystemAddress address);
	void HandleClientDisconnection(const SystemAddress address);
	short GetClientIndex(const SystemAddress address);
	short GetClientFreeSlot(void);
	void SendConnectionError(const SystemAddress address, const NetworkPlayerConnectionError error);
	NetworkPlayerFullUpdateData *GetPlayerFullUpdateData(const short index);
	NetworkVehicleFullUpdateData *GetVehicleFullUpdateData(const short index);
	template <typename DATATYPE>
	void SendDataToAll(const char *RPC, const DATATYPE *data);
	template <typename DATATYPE>
	void SendDataToOne(const char *RPC, const short index, const DATATYPE *data);
	template <typename DATATYPE>
	void SendDataToAllExceptOne(const char *RPC, const short index, const DATATYPE *data);
	//void SendClassInfo(const short client);
};
