/// \file
/// \brief Header file that describes the NetworkManager class.
/// \details See class description.
/// \author FaTony

#pragma once

#include "../../Shared/RakNet/RakPeerInterface.h"

#include "../../Shared/Network/NetworkProtocol.h"

/// \brief A network manager. It handles all network traffic.
/// \details TODO:
/// \author FaTony

class NetworkManager
{
public:
	NetworkManager(void);
	~NetworkManager(void);
	void Init(short maxclients, unsigned short port);
	void Tick(void);
	void CheckClients(void);
	void RemoveClient(const short index);
	void DisconnectClient(const short index);
	void RecieveClientConnection(const RPCParameters *rpcParameters);
	void RecievePlayerMove(const RPCParameters *rpcParameters);
	void RecievePlayerJump(const RPCParameters *rpcParameters);
	void RecievePlayerDuck(const RPCParameters *rpcParameters);
	void RecievePlayerEnterInVehicle(const RPCParameters *rpcParameters);
	void RecievePlayerCancelEnterInVehicle(const RPCParameters *rpcParameters);
	void RecievePlayerExitFromVehicle(const RPCParameters *rpcParameters);
	void RecievePlayerFire(const RPCParameters *rpcParameters);
	void RecievePlayerAim(const RPCParameters *rpcParameters);
	void RecievePlayerWeaponChange(const RPCParameters *rpcParameters);
	void RecievePlayerHealthAndArmourChange(const RPCParameters *rpcParameters);
	void RecievePlayerSpawnRequest(const RPCParameters *rpcParameters);
	void RecievePlayerModelChange(const RPCParameters *rpcParameters);
	void RecievePlayerComponentsChange(const RPCParameters *rpcParameters);
	void RecievePlayerChat(const RPCParameters *rpcParameters);
	bool SendNewVehicleInfoToAll(const short index);
private:
	unsigned short serverport;
	RakPeerInterface *net;
	short maxaddressbuffersize;
	short addressbuffersize;
	SystemAddress **addressbuffer;
	short RegisterNewClient(const SystemAddress address);
	short GetClientIndex(const SystemAddress address);
	short GetAddressFreeSlot(void);
	bool ResizeAddressBuffer(SystemAddress **&buffer, const short size);
	RakNet::BitStream *TranslateMessage(const RPCParameters *rpcParameters, short &index);
	void SendConnectionError(const SystemAddress address, const NetworkPlayerConnectionError error);
	NetworkPlayerFullUpdateData *GetPlayerFullUpdateData(const short index);
	NetworkVehicleFullUpdateData *GetVehicleFullUpdateData(const short index);
	void SendDataToAll(const char *RPC, const RakNet::BitStream *bsSend);
	void SendDataToAllExceptOne(const char *RPC, const short index, const RakNet::BitStream *bsSend);
	void SendClassInfo(const short client);
	void SendChatMessageToAll(const short client, const char *message, const unsigned char color[4]);
};
