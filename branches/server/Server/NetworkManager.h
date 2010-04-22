/// \file
/// \brief Header file that describes the NetworkManager class.
/// \details See class description.
/// \author FaTony

#pragma once

#include "net\RakPeerInterface.h"

enum ConnectionError
{
	ConnectionErrorServerFull,
	ConnectionErrorAllocationError,
	ConnectionErrorScriptLock
};

/// \brief A network manager. It handles all network traffic.
/// \details TODO:
/// \author FaTony

class NetworkManager
{
public:
	NetworkManager(void);
	~NetworkManager(void);
	void Init(unsigned char maxclients, unsigned short port);
	void Tick(void);
	void CheckClients(void);
	void RemoveClient(const unsigned char index);
	void SendNewPlayerInfoToAll(const unsigned char index, const char *name, const int model, const float position[3], const float angle, const short vehicleindex, const int seat_id, const int score, const int health, const int armour, const int room, const int weapons[8], const int ammo[8], const unsigned char color[4]);
	void SendNewPlayerInfo(const unsigned char client, const unsigned char index, const char *name, const int model, const float position[3], const float angle, const short vehicleindex, const int seat_id, const int score, const int health, const int armour, const int room, const int weapons[8], const int ammo[8], const unsigned char color[4]);
	void DisconnectClient(const unsigned char index);
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
	void SendConnectionError(const unsigned char index, const ConnectionError error);
	void SendVehicleInfo(const unsigned char client, const unsigned char index, const int model, const float position[3], const float angle, const int color[2]);
	void SendClassInfo(const unsigned char client);
	void SendNewVehicleToAll(const unsigned char index, const int model, const float position[3], const float angle, const int color[2]);
private:
	unsigned short serverport;
	RakPeerInterface *net;
	unsigned char maxaddressbuffersize;
	unsigned char addressbuffersize;
	SystemAddress **addressbuffer;
	bool RegisterNewClient(const SystemAddress address, unsigned char &index);
	bool GetClientIndex(const SystemAddress address, unsigned char &index);
	bool GetAddressFreeSlot(unsigned char &index);
	bool ResizeAddressBuffer(SystemAddress **&buffer, const unsigned char size);
	RakNet::BitStream *TranslateMessage(const RPCParameters *rpcParameters, unsigned char &index);
	void SendConnectionError(const SystemAddress address, const ConnectionError error);
	void SendUpdateToAll(const char *RPC, const unsigned char index, const RakNet::BitStream *bsSend);
	void SendChatMessageToAll(const unsigned char client, const char *message, const unsigned char color[4]);
};
