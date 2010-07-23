#include <stdio.h>
#include "CNetwork.h"
#include "log/log.h"

CNetwork::CNetwork()
{
	m_pNet = NULL;
	iMaxPlayers = 0;
}

CNetwork::~CNetwork()
{
	this->Unload();
}

bool CNetwork::Load(const unsigned short iPort, const unsigned short iMaxPlayers)
{
	m_pNet = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor s(iPort, 0);
	m_pNet->SetMaximumIncomingConnections(iMaxPlayers);
	m_pNet->Startup(iMaxPlayers, 1, &s, 1);

	m_pPlayerAddress = new SystemAddress[iMaxPlayers];
	memset(m_pPlayerAddress, 0, sizeof(SystemAddress) * iMaxPlayers);

	this->iMaxPlayers = iMaxPlayers;

	this->LoadBanList();
	return true;
}

bool CNetwork::Unload()
{
	if (m_pNet)
	{
		m_pNet->Shutdown(100,0);
		RakNetworkFactory::DestroyRakPeerInterface(m_pNet);
	}
	return true;
}

bool CNetwork::IsReady()
{
	return this->m_pNet != NULL;
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