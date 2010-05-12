#include "RakNetworkFactory.h"

#include "NetworkManager.h"

NetworkManager::NetworkManager(void)
{
	manager = NULL;
	rpc3 = NULL;
	net = NULL;
}

NetworkManager::~NetworkManager(void)
{
	if (net)
	{
		net->Shutdown(100,0);
		RakNetworkFactory::DestroyRakPeerInterface(net);
	}
	if (manager)
	{
		delete manager;
	}
	if (rpc3)
	{
		delete rpc3;
	}
}

void NetworkManager::Init(void)
{
	manager = new NetworkIDManager;
	clientid.localSystemAddress = 65535;
	serverid.localSystemAddress = 0;
	this->SetNetworkIDManager(manager);
	this->SetNetworkID(clientid);
	rpc3 = new RakNet::RPC3;
	rpc3->SetNetworkIDManager(manager);
	//RPC3_REGISTER_FUNCTION(rpc3, &NetworkManager::Function);
	net = RakNetworkFactory::GetRakPeerInterface();
	//SocketDescriptor socketDescriptor(50000,0);
	//net->Startup(1, 1, &socketDescriptor, 1);
	net->AttachPlugin(rpc3);
}