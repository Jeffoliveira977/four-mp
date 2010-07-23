#pragma once

#define FMP_PACKET_SIGNATURE 0xFF
#define FMP_NETWORK_VERSION 100

namespace NetworkData
{
	enum Types
	{
		NetworkConnect,
		NetworkConnectResult,
		NetworkDisconnect,
	};

	struct Connect
	{
		wchar_t nick[32];
		short version;
		char game;
	};

	struct ConnectResult
	{
		char result;
		short id;
	};

	struct Disconnect
	{
		short id;
	};
};