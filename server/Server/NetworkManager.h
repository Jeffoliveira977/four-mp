/// \file
/// \brief Header file that describes the NetworkManager class.
/// \details See class description.
/// \author WNeZRoS, FaTony

#pragma once

#include "NetworkIDObject.h"
#include "NetworkIDManager.h"
#include "../../Shared/RPC3/RPC3.h"
#include "RakPeerInterface.h"

#include "../../Shared/Network/NetworkProtocol.h"

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
	short RegisterNewClient(const SystemAddress address);
	short GetClientIndex(const SystemAddress address);
	short GetClientFreeSlot(void);
	void SendConnectionError(const SystemAddress address, const NetworkPlayerConnectionError error);
	NetworkPlayerFullUpdateData *GetPlayerFullUpdateData(const short index);
	NetworkVehicleFullUpdateData *GetVehicleFullUpdateData(const short index);
	template <typename DATATYPE>
	void SendDataToAll(const char *RPC, const DATATYPE *data);
	template <typename DATATYPE>
	void SendDataToAllExceptOne(const char *RPC, const short index, const DATATYPE *data);
	//void SendClassInfo(const short client);
};
