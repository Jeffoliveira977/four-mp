#pragma once
#include <vector>
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

	bool Load(const unsigned short iPort, const unsigned short iMaxPlayers);
	bool Unload();

	void Tick();
	bool IsReady();

	void CloseConnection(const SystemAddress addr);
	void AddToBanList(const SystemAddress addr, wchar_t * pszNick);
	void ClearBanList();
	void ReloadBanList();
	void LoadBanList();

private:
	RakPeerInterface * m_pNet;
	std::vector<wchar_t *> m_vBadNick;
	SystemAddress * m_pPlayerAddress;
	short iMaxPlayers;

	template <typename DATATYPE> 
	bool SendTo(const short iPlayer, const DATATYPE * pData, const NetworkData::Types iType, const char PackPriority = 2);
	template <typename DATATYPE> 
	bool SendToAll(const DATATYPE * pData, const NetworkData::Types iType, const short iExceptPlayer = -1, const char PackPriority = 2);
	template <typename DATATYPE> 
	bool SendTo(const SystemAddress sysAddr, const DATATYPE * pData, const NetworkData::Types iType, const char PackPriority = 2);
};