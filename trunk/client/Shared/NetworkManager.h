#pragma once

#include <windows.h>
#include "NetworkIDObject.h"

#include "../../Shared/Network/NetworkProtocol.h"

namespace RakNet
{
	class RPC3;
}

class NetworkManager : public NetworkIDObject
{
public:
	NetworkManager(void);
	~NetworkManager(void);
	void Load(void);
	void Tick(void);
	void Unload(void);
	void Ping(const char *hostname, const unsigned short port = 7777);
	void ConnectToServer(const char *hostname = "", const unsigned short port = 7777);

	void SendClientConnectionRequest(void);
	void SendPlayerMove(const float speed);
	void SendPlayerJump(void);
	void SendPlayerDuck(void);
	void SendPlayerStartEntranceInVehicle(void);
	void SendPlayerCancelEntranceInVehicle(void);
	void SendPlayerFinishEntranceInVehicle(void);
	void SendPlayerStartExitFromVehicle(void);
	void SendPlayerFinishExitFromVehicle(void);
	void SendPlayerFire(const float position[3], const int time, const short target, const unsigned char health, const unsigned int armor);
	void SendPlayerAim(const float position[3], const int time);
	void SendPlayerWeaponChange(void);
	void SendPlayerHealthAndArmorChange(void);
	void SendPlayerSpawnRequest(void);
	void SendPlayerModelChange(void);
	void SendPlayerComponentsChange(void);
	void SendPlayerChat(const wchar_t *message);
	
	void RecieveClientConnection(NetworkPlayerFullUpdateData data, RakNet::RPC3 *serverrpc3);
	void RecieveClientConnectionError(NetworkPlayerConnectionErrorData data, RakNet::RPC3 *serverrpc3);
	void RecieveClientInfo(NetworkPlayerInfoData data, RakNet::RPC3 *serverrpc3);
	void RecieveClientDisconnection(NetworkPlayerDisconnectionData data, RakNet::RPC3 *serverrpc3);
	void RecieveGameTimeChange(NetworkGameTimeChangeData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerFullUpdate(NetworkPlayerFullUpdateData data, RakNet::RPC3 *serverrpc3);
	void RecieveVehicleFullUpdate(NetworkVehicleFullUpdateData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerMove(NetworkPlayerMoveData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerJump(NetworkPlayerJumpData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerDuck(NetworkPlayerDuckData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerStartEntranceInVehicle(NetworkPlayerStartEntranceInVehicleData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerCancelEntranceInVehicle(NetworkPlayerCancelEntranceInVehicleData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerFinishEntranceInVehicle(NetworkPlayerFinishEntranceInVehicleData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerStartExitFromVehicle(NetworkPlayerStartExitFromVehicleData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerFinishExitFromVehicle(NetworkPlayerFinishExitFromVehicleData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerFire(NetworkPlayerFireData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerAim(NetworkPlayerAimData, RakNet::RPC3 *serverrpc3);
	void RecievePlayerWeaponChange(NetworkPlayerWeaponChangeData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerHealthAndArmorChange(NetworkPlayerHealthAndArmorChangeData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerSpawnPositionChange(NetworkPlayerSpawnPositionChangeData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerSpawn(NetworkPlayerSpawnData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerModelChange(NetworkPlayerModelChangeData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerComponentsChange(NetworkPlayerComponentsChangeData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerChat(NetworkPlayerChatData data, RakNet::RPC3 *serverrpc3);
	void RecieveNewVehicle(NetworkVehicleFullUpdateData data, RakNet::RPC3 *serverrpc3);
	void RecievePlayerPosition(NetworkPlayerPositionData data, RakNet::RPC3 *serverrpc3);
private:
	NetworkIDManager *manager;
	NetworkID clientid;
	NetworkID serverid;
	SystemAddress serveraddress;
	RakNet::RPC3 *rpc3;
	RakPeerInterface *net;
	enum NetworkRPCType
	{
		NetworkRPCPlayerConnection,
		NetworkRPCPlayerConnectionError,
		NetworkRPCPlayerInfo,
		NetworkRPCPlayerDisconnection,
		NetworkRPCGameTimeChange,
		NetworkRPCPlayerFullUpdate,
		NetworkRPCVehicleFullUpdate,
		NetworkRPCPlayerMove,
		NetworkRPCPlayerJump,
		NetworkRPCPlayerDuck,
		NetworkRPCPlayerStartEntranceInVehicle,
		NetworkRPCPlayerCancelEntranceInVehicle,
		NetworkRPCPlayerFinishEntranceInVehicle,
		NetworkRPCPlayerStartExitFromVehicle,
		NetworkRPCPlayerFinishExitFromVehicle,
		NetworkRPCPlayerFire,
		NetworkRPCPlayerAim,
		NetworkRPCPlayerWeaponChange,
		NetworkRPCPlayerHealthAndArmorChange,
		NetworkRPCPlayerSpawnPositionChange,
		NetworkRPCPlayerSpawn,
		NetworkRPCPlayerModelChange,
		NetworkRPCPlayerComponentsChange,
		NetworkRPCPlayerChat,
		NetworkRPCNewVehicle,
		NetworkRPCPlayerPosition
	};
	union NetworkRPCUnion
	{
		NetworkPlayerFullUpdateData *playerconnection;
		NetworkPlayerConnectionErrorData *playerconnectionerror;
		NetworkPlayerInfoData *playerinfo;
		NetworkPlayerDisconnectionData *playerdisconnection;
		NetworkGameTimeChangeData *gametimechange;
		NetworkPlayerFullUpdateData *playerfullupdate;
		NetworkVehicleFullUpdateData *vehiclefullupdate;
		NetworkPlayerMoveData *playermove;
		NetworkPlayerJumpData *playerjump;
		NetworkPlayerDuckData *playerduck;
		NetworkPlayerStartEntranceInVehicleData *playerstartentranceinvehicle;
		NetworkPlayerCancelEntranceInVehicleData *playercancelentranceinvehicle;
		NetworkPlayerFinishEntranceInVehicleData *playerfinishentranceinvehicle;
		NetworkPlayerStartExitFromVehicleData *playerstartexitfromvehicle;
		NetworkPlayerFinishExitFromVehicleData *playerfinishexitfromvehicle;
		NetworkPlayerFireData *playerfire;
		NetworkPlayerAimData *playeraim;
		NetworkPlayerWeaponChangeData *playerweaponchange;
		NetworkPlayerHealthAndArmorChangeData *playerhealthandarmorchange;
		NetworkPlayerSpawnPositionChangeData *playerspawnpositionchange;
		NetworkPlayerSpawnData *playerspawn;
		NetworkPlayerModelChangeData *playermodelchange;
		NetworkPlayerComponentsChangeData *playercomponentschange;
		NetworkPlayerChatData *playerchat;
		NetworkVehicleFullUpdateData *newvehicle;
		NetworkPlayerPositionData *playerpos;
	};
	struct NetworkRPCData
	{
		NetworkRPCType type;
		NetworkRPCUnion data;
	};
	int maxrpcbuffersize;
	int rpcbuffersize;
	NetworkRPCData *rpcbuffer;
	CRITICAL_SECTION rpcbuffersection;
	template <typename DATATYPE>
	void WriteToRPCBuffer(const NetworkRPCType type, const DATATYPE *data);
	void HandleRPCData(const NetworkRPCType type, const NetworkRPCUnion *data);
	void FreeRPCBuffer(void);
};
