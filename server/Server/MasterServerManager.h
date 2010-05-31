#pragma once

class TCPInterface;
class HTTPConnection;
namespace RakNet
{
	class RakString;
}

#define MASTER_HOST "master.four-mp.com"
#define MASTER_PATH "/"

class MasterServerManager
{
	bool isloaded;
	TCPInterface *tcp;
	HTTPConnection *http;
	enum MasterServerStates
	{
		MSS_NONE,
		MSS_ERROR,
		MSS_WAIT_SERVER_ADD,
		MSS_WAIT_USER_CHECK,
		MSS_WAIT_CLAN_CHECK
	};
	MasterServerStates state;
protected:
	void Process(void);
	void ReadPacket(RakNet::RakString data);
public:
	MasterServerManager(void);
	~MasterServerManager(void);
	bool Init(void);
	bool RegisterServer(const unsigned short port, const char *name, const char *mode, const char *loc, const short maxplayers, const bool password);
	bool QueryUserCheck(const int fmpid, const char *ip, const char *seskey);
	bool QueryClanCheck(const int fmpid, const char *clan);
};
