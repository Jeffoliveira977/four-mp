/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/
#include "NetworkManager.h"

extern NetworkManager nm;

void RPC_ClientConnect(RPCParameters *rpcParameters)
{
	nm.RecieveClientConnection(rpcParameters);
}

void RPC_Check(RPCParameters *rpcParameters)
{
	//TODO: Removed as there was no meaningful code.
	//TODO: wtf should be here anyway?
}

void RPC_MovePlayer(RPCParameters *rpcParameters)
{
	nm.RecievePlayerMove(rpcParameters);
}

void RPC_JumpPlayer(RPCParameters *rpcParameters)
{
	nm.RecievePlayerJump(rpcParameters);
}

void RPC_DuckPlayer(RPCParameters *rpcParameters)
{
	nm.RecievePlayerDuck(rpcParameters);
}

void RPC_PlayerEnterInVehicle(RPCParameters *rpcParameters)
{
	nm.RecievePlayerEntranceInVehicle(rpcParameters);
}

void RPC_PlayerCancelEnterInVehicle(RPCParameters *rpcParameters)
{
	nm.RecievePlayerCancelEntranceInVehicle(rpcParameters);
}

void RPC_PlayerExitFromVehicle(RPCParameters *rpcParameters)
{
	nm.RecievePlayerExitFromVehicle(rpcParameters);
}

void RPC_PlayerFire(RPCParameters *rpcParameters)
{
	nm.RecievePlayerFire(rpcParameters);
}

void RPC_PlayerAim(RPCParameters *rpcParameters)
{
	nm.RecievePlayerAim(rpcParameters);
}

void RPC_SwapGun(RPCParameters *rpcParameters)
{
	nm.RecievePlayerWeaponChange(rpcParameters);
}

void RPC_PlayerParams(RPCParameters *rpcParameters)
{
	nm.RecievePlayerHealthAndArmorChange(rpcParameters);
}

void RPC_PlayerSpawn(RPCParameters *rpcParameters)
{
	nm.RecievePlayerSpawnRequest(rpcParameters);
}
void RPC_Select_ModelChanged(RPCParameters *rpcParameters)
{
	nm.RecievePlayerModelChange(rpcParameters);
}
void RPC_SyncSkinVariation(RPCParameters *rpcParameters)
{
	nm.RecievePlayerComponentsChange(rpcParameters);
}

void RPC_Chat(RPCParameters *rpcParameters)
{
	nm.RecievePlayerChat(rpcParameters);
}