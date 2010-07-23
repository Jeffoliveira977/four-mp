#include <stdio.h>
#include "CNetwork.h"
#include "log/log.h"

template <typename DATATYPE>
bool CNetwork::SendTo(const short iPlayer, const DATATYPE * pData, const NetworkData::Types iType, const char PackPriority)
{
	if(pData == NULL) return false;
	if(iPlayer > iMaxPlayers || iPlayer < 0) return false;
	if(m_pPlayerAddress[iPlayer].port == 0) return false;

	int iSize = sizeof(DATATYPE);
	if(iSize == 4) Log::Warning("Send %d: sizeof(pData) == 4", iType);

    char * pszData = new char[iSize + 3];
    pszData[0] = (char)FMP_PACKET_SIGNATURE;
    *(short*)(pszData + 1) = iType;
    memcpy(pszData + 3, pData, iSize);

	m_pNet->Send(pszData, iSize + 3, (PacketPriority)PackPriority, RELIABLE_ORDERED, NULL, m_pPlayerAddress[iPlayer], false);

	delete pszData;
	return true;
}

template <typename DATATYPE>
bool CNetwork::SendToAll(const DATATYPE * pData, const NetworkData::Types iType, const short iExceptPlayer, const char PackPriority)
{
	if(pData == NULL) return false;

	int iSize = sizeof(DATATYPE);
	if(iSize == 4) Log::Warning("Send %d: sizeof(pData) == 4", iType);

    char * pszData = new char[iSize + 3];
    pszData[0] = (char)FMP_PACKET_SIGNATURE;
    *(short*)(pszData + 1) = iType;
    memcpy(pszData + 3, pData, iSize);

	for(int i = 0; i < iMaxPlayers; i++)
		if(i != iExceptPlayer)
			m_pNet->Send(pszData, iSize + 3, (PacketPriority)PackPriority, RELIABLE_ORDERED, NULL, m_pPlayerAddress[i], false);

	delete pszData;
	return true;
}

template <typename DATATYPE>
bool CNetwork::SendTo(const SystemAddress sysAddr, const DATATYPE * pData, const NetworkData::Types iType, const char PackPriority)
{
	if(pData == NULL) return false;

	int iSize = sizeof(DATATYPE);
	if(iSize == 4) Log::Warning("Send %d: sizeof(pData) == 4", iType);

    char * pszData = new char[iSize + 3];
    pszData[0] = (char)FMP_PACKET_SIGNATURE;
    *(short*)(pszData + 1) = iType;
    memcpy(pszData + 3, pData, iSize);

	m_pNet->Send(pszData, iSize + 3, (PacketPriority)PackPriority, RELIABLE_ORDERED, NULL, sysAddr, false);

	delete pszData;
	return false;
}