#pragma once
#include "RakNet/RakPeerInterface.h"
#include "RakNet/RakNetworkFactory.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/MessageIdentifiers.h"

#define FMP_PACKET_SIGNATURE 0xFF

class CNetwork
{
public:
	CNetwork();
	~CNetwork();

	bool Connect(const char * pszServer, const unsigned short iPort, const char * pszPassword = NULL);

	void Tick();
	bool IsReady();
	bool IsConnected();

	template <typename DATATYPE>
	void Send(const DATATYPE * pData, const short iType, const char PackPriority = 2);

private:
	RakPeerInterface * m_pNet;
	SystemAddress m_serverAddress;
};