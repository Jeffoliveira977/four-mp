#include <stdio.h>
#include "CNetwork.h"
#include "log/log.h"

CNetwork::CNetwork()
{
	m_pNet = NULL;
	m_pNet = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor socketDescriptor(0,0);
	m_pNet->Startup(1, 1, &socketDescriptor, 1);
}

CNetwork::~CNetwork()
{
	if (m_pNet)
	{
		m_pNet->Shutdown(100,0);
		RakNetworkFactory::DestroyRakPeerInterface(m_pNet);
	}
}

bool CNetwork::Connect(const char * pszServer, const unsigned short iPort, const char * pszPassword)
{
	m_serverAddress.SetBinaryAddress(pszServer);
	m_serverAddress.port = iPort;

	return m_pNet->Connect(pszServer, iPort, pszPassword, strlen(pszPassword));
}

bool CNetwork::IsReady()
{
	return this->m_pNet != NULL;
}

bool CNetwork::IsConnected()
{
	return m_pNet->IsConnected(m_serverAddress);
}

template <typename DATATYPE>
void CNetwork::Send(const DATATYPE * pData, const short iType, const char PackPriority)
{
	int iSize = sizeof(DATATYPE);
	if(iSize == 4) Log::Warning("Send %d: sizeof(pData) == 4", iType);

    char * pszData = new char[iSize + 3];
    pszData[0] = (char)FMP_PACKET_SIGNATURE;
    *(short*)(pszData + 1) = iType;
    memcpy(pszData + 3, pData, iSize);

	m_pNet->Send(pszData, iSize + 3, (PacketPriority)PackPriority, RELIABLE_ORDERED, NULL, m_serverAddress, false);

	delete pszData;
}

void CNetwork::Tick()
{
	static Packet * pPack;
	for (pPack = m_pNet->Receive(); pPack; m_pNet->DeallocatePacket(pPack), pPack = m_pNet->Receive())
	{
		switch (pPack->data[0])
		{
		case FMP_PACKET_SIGNATURE:
			{
				//this->HandlePacket(pack->data + 1, pack->length - 1, pack->systemAddress);
			} break;
		case ID_CONNECTION_REQUEST:
			{
				Log::Debug(L"New connection request");
			} break;
		case ID_NEW_INCOMING_CONNECTION:
			{
				Log::Debug(L"New connection from %S", pPack->systemAddress.ToString(1));
			} break;
		case ID_DISCONNECTION_NOTIFICATION:
			{
				//this->HandleClientDisconnection(pack->systemAddress);
			} break;
		case ID_CONNECTION_LOST:
			{
				//this->HandleClientDisconnection(pack->systemAddress);
			} break;
		}
	}
}