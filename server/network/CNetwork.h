#pragma once
#include <vector>
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

	bool Load(unsigned short iPort, unsigned short iMaxPlayers);
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
};