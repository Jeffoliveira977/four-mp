#include <vector>
#include "RakString.h"

class TCPInterface;
class HTTPConnection;

#define MASTER_HOST "master.four-mp.com"
#define MASTER_PATH "/"

struct MasterServerInfo
{
	char name[128];			// Server's name
	char ip[64];			// Server's IP
	unsigned short port;				// Server's port
	char loc[64];			// Server's location
	char mode[64];			// Server's game mode
	short players;			// Count players on server
	short maxplayers;			// Count slots on server
	bool password;			// Server have a password?
	char clan[64];			// Clan's server
	bool ban;				// Server in black list?
	bool vip;				// Server is VIP?
	unsigned int ping;	// Ping
};

// Information of player
struct MasterUserInfo
{
	int fmpid;				// FOUR-MP ID
	char name[32];			// Player nick
	char login[32];			// Player login
	char status[128];		// Player status
	char seskey[32];		// Session key hash
	std::string friends;			// Player's friends
};

// Query states
enum MasterServerStates
{
	MSS_NONE,
	MSS_ERROR,
	MSS_WAIT_SERVER_LIST,
	MSS_WAIT_USER_LOGIN,
	MSS_WAIT_USER_UPDATE,
	MSS_WAIT_USER_REGISTER,
	MSS_WAIT_USER_LOGOUT,
	MSS_WAIT_CHECK,
	MSS_WAIT_CLAN
};

// Class for four-mp master server
class MasterServer
{
	TCPInterface *tcp;
	HTTPConnection *http;

	std::vector<MasterServerInfo*> slist;
	MasterUserInfo *user;

	MasterServerStates state;
protected:
	void Process();
	void ReadPacket(RakNet::RakString data);

	void ReadServerList(RakNet::RakString data);
	void ReadUserInfo(RakNet::RakString data);
	void ReadClanInfo(RakNet::RakString data);
public:
	MasterServer();
	~MasterServer();
	void Load();

	void ClearServerList();

	bool QueryServerList(bool ban = 0, bool vip = 0, bool empty = 0, bool full = 0, bool password = 1, const char *clan = 0, const char *name = 0, const char *mode = 0, const char *loc = 0);
	bool QueryUserLogin(const char *login, const char *password);
	bool QueryUserUpdate(const char *status);
	bool QueryUserRegister(const char *login, const char *nick, const char *password, const char *email);
	bool QueryUserLogout();
	bool QueryClan();

	std::vector<MasterServerInfo*> *GetServerList();
	MasterServerInfo *GetServerInfo(int Index);
	MasterServerInfo *GetServerInfo(char *ip, unsigned short port, int *index);
	int GetNumServers();
	bool IsServerVIP(char *ip, unsigned short port);
	bool IsServerBanned(char *ip, unsigned short port);

	bool IsUserInfo();
	MasterUserInfo *GetUserInfo();
	int GetUserId();
	char *GetUserName();
};