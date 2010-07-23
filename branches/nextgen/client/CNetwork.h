#pragma once
#include "RakNet/RakPeerInterface.h"
#include "RakNet/RakNetworkFactory.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/MessageIdentifiers.h"
#include "NetworkData.h"

class CNetwork
{
public:
	CNetwork();
	~CNetwork();

	bool Connect(const char * pszServer, const unsigned short iPort, const char * pszPassword = NULL);
	bool Ping(const char * pszServer, const unsigned short iPort);

	void Tick();
	bool IsReady();
	bool IsConnected();

private:
	RakPeerInterface * m_pNet;
	SystemAddress m_serverAddress;

	template <typename DATATYPE>
	void Send(const DATATYPE * pData, const NetworkData::Types iType, const char PackPriority = 2);
};