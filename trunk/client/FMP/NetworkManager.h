#pragma once

#include "NetworkIDObject.h"
#include "NetworkIDManager.h"
#include "../../Shared/RPC3/RPC3.h"
#include "RakPeerInterface.h"

class NetworkManager : public NetworkIDObject
{
public:
	NetworkManager(void);
	~NetworkManager(void);
	void Init(void);
private:
	NetworkIDManager *manager;
	NetworkID clientid;
	NetworkID serverid;
	RakNet::RPC3 *rpc3;
	RakPeerInterface *net;
};
