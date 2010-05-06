/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

void RPC_ClientConnect(RPCParameters *rpcParameters);
void RPC_Check(RPCParameters *rpcParameters);

void RPC_MovePlayer(RPCParameters *rpcParameters);
void RPC_JumpPlayer(RPCParameters *rpcParameters);
void RPC_DuckPlayer(RPCParameters *rpcParameters);

void RPC_PlayerCancelEnterInVehicle(RPCParameters *rpcParameters);
void RPC_PlayerExitFromVehicle(RPCParameters *rpcParameters);
void RPC_PlayerEnterInVehicle(RPCParameters *rpcParameters);

void RPC_PlayerFire(RPCParameters *rpcParameters);
void RPC_PlayerAim(RPCParameters *rpcParameters);
void RPC_SwapGun(RPCParameters *rpcParameters);
void RPC_PlayerParams(RPCParameters *rpcParameters);

void RPC_PlayerSpawn(RPCParameters *rpcParameters);
void RPC_Select_ModelChanged(RPCParameters *rpcParameters);
void RPC_SyncSkinVariation(RPCParameters *rpcParameters);

void RPC_Chat(RPCParameters *rpcParameters);