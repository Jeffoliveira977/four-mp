#include <stdio.h>
#include "CNetwork.h"
#include "../main.h"
#include "../log/log.h"

CNetwork::CNetwork()
{
	m_pNet = NULL;
}

CNetwork::~CNetwork()
{
	this->Unload();
}

bool CNetwork::Load(unsigned short iPort, unsigned short iMaxPlayers)
{
	m_pNet = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor s(iPort, 0);
	m_pNet->SetMaximumIncomingConnections(iMaxPlayers);
	m_pNet->Startup(iMaxPlayers, 1, &s, 1);

	this->LoadBanList();
	return true;
}

bool CNetwork::Unload()
{
	return true;
}

bool CNetwork::IsReady()
{
	return this->m_pNet != NULL;
}

void CNetwork::Tick()
{
	static Packet *pack;
	for (pack = m_pNet->Receive(); pack; m_pNet->DeallocatePacket(pack), pack = m_pNet->Receive())
	{
		switch (pack->data[0])
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
				Log::Debug(L"New connection from %S", pack->systemAddress.ToString(1));
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