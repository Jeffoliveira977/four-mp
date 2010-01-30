/*
*	Created:			29.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/
#pragma once
#include "net\RakPeerInterface.h"

void Check(RPCParameters *rpcParameters);
void ErrorConnect(RPCParameters *rpcParameters);
void ConnectPlayer(RPCParameters *rpcParameters);
void MovePlayer(RPCParameters *rpcParameters);
void JumpPlayer(RPCParameters *rpcParameters);
void DuckPlayer(RPCParameters *rpcParameters);
void Disconnect(RPCParameters *rpcParameters);

void CreateVehicle(RPCParameters *rpcParameters);

void PlayerCancelEnterInVehicle(RPCParameters *rpcParameters);
void PlayerExitFromVehicle(RPCParameters *rpcParameters);
void PlayerEnterInVehicle(RPCParameters *rpcParameters);

void PlayerFire(RPCParameters *rpcParameters);
void PlayerAim(RPCParameters *rpcParameters);
void SwapGun(RPCParameters *rpcParameters);

void PlayerParams(RPCParameters *rpcParameters);

void ClassSync(RPCParameters *rpcParameters);
void SyncSkin(RPCParameters *rpcParameters);
void SyncSkinVariation(RPCParameters *rpcParameters);
void PlayerSpawn(RPCParameters *rpcParameters);

void Chat(RPCParameters *rpcParameters);