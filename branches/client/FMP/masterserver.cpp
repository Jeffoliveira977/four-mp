#include "masterserver.h"

MasterServer::MasterServer()
{
	
}

void MasterServer::Load()
{
	tcp = RakNet::OP_NEW<TCPInterface>(__FILE__,__LINE__);
	http = RakNet::OP_NEW<HTTPConnection>(__FILE__,__LINE__);

	tcp->Start(0, 64);
	http->Init(tcp, MASTER_HOST);

	state = MSS_NONE;
	user = new MasterUserInfo;
	memset(user, 0, sizeof(MasterUserInfo));
}

MasterServer::~MasterServer()
{
	free(http);
	free(tcp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool MasterServer::QueryServerList(bool ban, bool vip, bool empty, bool full, bool password, const char *clan, const char *name, const char *mode, const char *loc)
{
	// t=list
	// filter[%s] : ban, vip, name, mode, empty, full, clan, password, loc

	if(http->IsBusy()) return 0;

	RakString data, tmp;
	if(ban) data += "ban=1&";
	if(vip) data += "vip=1&"; 
	if(empty) data += "empty=1&"; 
	if(full) data += "full=1&"; 
	if(password) data += "password=1&"; 
	if(clan) { tmp = clan; tmp.URLEncode(); data += RakString("clan=%s&", tmp.C_String()); } 
	if(name) { tmp = name; tmp.URLEncode(); data += RakString("name=%s&", tmp.C_String()); } 
	if(mode) { tmp = mode; tmp.URLEncode(); data += RakString("mode=%s&", tmp.C_String()); } 
	if(loc) { tmp = loc; tmp.URLEncode(); data += RakString("loc=%s&", tmp.C_String()); } 
	data += "t=list";

	http->Post(RakString("%s%s", MASTER_PATH, "list.php").C_String(), data.C_String());
	state = MSS_WAIT_SERVER_LIST;

	RakNetTimeMS endTime=RakNet::GetTimeMS() + 10000;
	while(RakNet::GetTimeMS() < endTime && !(state == MSS_NONE || state == MSS_ERROR))
	{
		Process();
		Sleep(50);
	}

	if(state != MSS_NONE)
	{
		state = MSS_NONE;
		return 0;
	}

	return 1;
}

bool MasterServer::QueryUserLogin(const char *login, const char *password)
{
	// t=login
	// login= &pass=

	if(http->IsBusy()) return 0;

	RakString data = RakString("login=%s&pass=%s&t=login", 
		RakString(login).URLEncode().C_String(), RakString(password).URLEncode().C_String());

	http->Post(RakString("%s%s", MASTER_PATH, "login.php").C_String(), data);
	state = MSS_WAIT_USER_LOGIN;
	strcpy(user->login, login);

	RakNetTimeMS endTime=RakNet::GetTimeMS() + 10000;
	while(RakNet::GetTimeMS() < endTime && !(state == MSS_NONE || state == MSS_ERROR))
	{
		Process();
		Sleep(50);
	}

	if(state != MSS_NONE)
	{
		state = MSS_NONE;
		return 0;
	}

	return 1;
}

bool MasterServer::QueryUserRegister(const char *login, const char *nick, const char *password, const char *email)
{
	// t=reg
	// login= &pass= &email= &nick= &hard=

	if(http->IsBusy()) return 0;

	RakString data = RakString("login=%s&pass=%s&email=%s&nick=%s&hard=%s&t=reg", 
		RakString(login).URLEncode().C_String(), RakString(password).URLEncode().C_String(),
		RakString(email).URLEncode().C_String(), RakString(nick).URLEncode().C_String(), 
		RakString("fmp02a").URLEncode().C_String());

	http->Post(RakString("%s%s", MASTER_PATH, "login.php").C_String(), data);
	state = MSS_WAIT_USER_REGISTER;

	RakNetTimeMS endTime=RakNet::GetTimeMS() + 10000;
	while(RakNet::GetTimeMS() < endTime && !(state == MSS_NONE || state == MSS_ERROR))
	{
		Process();
		Sleep(50);
	}

	if(state != MSS_NONE)
	{
		state = MSS_NONE;
		return 0;
	}

	return 1;
}

bool MasterServer::QueryUserLogout()
{
	// t=logout
	// fmpid= &seskey=

	if(http->IsBusy()) return 0;

	RakString data = RakString("fmpid=%d&seskey=%s&t=logout", user->fmpid, 
		RakString(user->seskey).URLEncode().C_String());

	http->Post(RakString("%s%s", MASTER_PATH, "login.php").C_String(), data);
	state = MSS_WAIT_USER_LOGOUT;

	RakNetTimeMS endTime=RakNet::GetTimeMS() + 10000;
	while(RakNet::GetTimeMS() < endTime && !(state == MSS_NONE || state == MSS_ERROR))
	{
		Process();
		Sleep(50);
	}

	if(state != MSS_NONE)
	{
		state = MSS_NONE;
		return 0;
	}

	return 1;
}

bool MasterServer::QueryUserUpdate(const char *status)
{
	// t=logout
	// fmpid= &seskey= &status=

	if(http->IsBusy()) return 0;

	RakString data = RakString("fmpid=%d&seskey=%s&status=%s&t=update", user->fmpid, 
		RakString(user->seskey).URLEncode().C_String(), RakString(status).URLEncode().C_String());

	http->Post(RakString("%s%s", MASTER_PATH, "login.php").C_String(), data);
	state = MSS_WAIT_USER_UPDATE;

	RakNetTimeMS endTime=RakNet::GetTimeMS() + 10000;
	while(RakNet::GetTimeMS() < endTime && !(state == MSS_NONE || state == MSS_ERROR))
	{
		Process();
		Sleep(50);
	}

	if(state != MSS_NONE)
	{
		state = MSS_NONE;
		return 0;
	}

	return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MasterServer::ReadPacket(RakString data)
{

	switch(state)
	{
	case MSS_WAIT_SERVER_LIST: ReadServerList(data); break;
	case MSS_WAIT_USER_LOGIN: ReadUserInfo(data); break;
	case MSS_WAIT_USER_REGISTER: ReadUserInfo(data); break;
	case MSS_WAIT_USER_LOGOUT: ReadUserInfo(data); break;
	case MSS_WAIT_USER_UPDATE: ReadUserInfo(data); break;
	case MSS_WAIT_CLAN: ReadClanInfo(data); break;
	default: break;
	}

}

void MasterServer::ReadServerList(RakString data)
{
	const char *dataCode = data.C_String();
	// `ip`, `port`, `name`, `location`, `mode`, `password`, `players`, `maxplayers`, `clan`, `ban`, `vip`

	MasterServerInfo tmp_info;
	memset(&tmp_info, 0, sizeof(MasterServerInfo));

	unsigned char read_id = 0;

	for(int i = 1; i < (int)strlen(dataCode); i++)
	{
		if(dataCode[i] == 1) 
		{ 
			read_id++; 
			continue; 
		}
		else if(dataCode[i] == 2) 
		{ 
			read_id = 0;
			list.push_back(tmp_info);
			memset(&tmp_info, 0, sizeof(MasterServerInfo));
			continue;
		}
		
		if(read_id == 0)
		{
			int len = strlen(tmp_info.ip);
			tmp_info.ip[len] = dataCode[i];
			tmp_info.ip[len+1] = 0;
		}
		else if(read_id == 1)
		{
			tmp_info.port = tmp_info.port*10 + dataCode[i]-48;
		}
		else if(read_id == 2)
		{
			int len = strlen(tmp_info.name);
			tmp_info.name[len] = dataCode[i];
			tmp_info.name[len+1] = 0;
		}
		else if(read_id == 3)
		{
			int len = strlen(tmp_info.loc);
			tmp_info.loc[len] = dataCode[i];
			tmp_info.loc[len+1] = 0;
		}
		else if(read_id == 4)
		{
			int len = strlen(tmp_info.mode);
			tmp_info.mode[len] = dataCode[i];
			tmp_info.mode[len+1] = 0;
		}
		else if(read_id == 5)
		{
			tmp_info.password = dataCode[i] != 48;
		}
		else if(read_id == 6)
		{
			tmp_info.players = tmp_info.players*10 + dataCode[i]-48;
		}
		else if(read_id == 7)
		{
			tmp_info.maxplayers = tmp_info.maxplayers*10 + dataCode[i]-48;
		}
		else if(read_id == 8)
		{
			int len = strlen(tmp_info.clan);
			tmp_info.clan[len] = dataCode[i];
			tmp_info.clan[len+1] = 0;
		}
		else if(read_id == 9)
		{
			tmp_info.ban = dataCode[i] != 48;
		}
		else if(read_id == 10)
		{
			tmp_info.vip = dataCode[i] != 48;
		}
	}

	state = MSS_NONE;
}

void MasterServer::ReadUserInfo(RakString data)
{
	const char *dataCode = data.C_String();

	if(state == MSS_WAIT_USER_LOGIN)
	{
		// fmpid session_key nick status friends
		if(dataCode[1] == 'E') { state = MSS_ERROR; return; }

		unsigned char read_id = 0;
		user->friends.clear();

		for(int i = 1; i < (int)strlen(dataCode); i++)
		{
			if(dataCode[i] == 1) 
			{ 
				read_id++; 
				continue; 
			}

			if(read_id == 0)
			{
				user->fmpid = user->fmpid*10 + dataCode[i]-48;
			}
			else if(read_id == 1)
			{
				int len = strlen(user->seskey);
				user->seskey[len] = dataCode[i];
				user->seskey[len+1] = 0;
			}
			else if(read_id == 2)
			{
				int len = strlen(user->name);
				user->name[len] = dataCode[i];
				user->name[len+1] = 0;
			}
			else if(read_id == 3)
			{
				int len = strlen(user->status);
				user->status[len] = dataCode[i];
				user->status[len+1] = 0;
			}
			else if(read_id == 4)
			{
				user->friends.push_back(dataCode[i]);
			}
		}
		state = MSS_NONE;
	}
	else if(state == MSS_WAIT_USER_LOGOUT)
	{
		if(dataCode[1] == 'O') state = MSS_NONE;
		else state = MSS_ERROR;
	}
	else if(state == MSS_WAIT_USER_REGISTER)
	{
		if(dataCode[1] == 'O') state = MSS_NONE;
		else state = MSS_ERROR;
	}
	else if(state == MSS_WAIT_USER_UPDATE)
	{
		if(dataCode[1] == 'O') state = MSS_NONE;
		else state = MSS_ERROR;
	}
}

void MasterServer::ReadClanInfo(RakString data)
{
	//printf("%s\n", data.C_String());
	state = MSS_NONE;
}

void MasterServer::Process()
{
	if(!tcp) return;

	Packet *packet = tcp->Receive();
	if(packet) 
	{
		http->ProcessTCPPacket(packet);
		tcp->DeallocatePacket(packet);
	}
	if(http->HasRead())
	{
		int code;
		RakNet::RakString data;
		if(http->HasBadResponse(&code, &data))
		{
			printf("Error code %d: %s", code, data.C_String());
			state = MSS_ERROR;
		}
		else
			ReadPacket(http->Read());
	}

	http->Update();
}

void MasterServer::ClearServerList()
{
	list.clear();
}

vector<MasterServerInfo> *MasterServer::GetServerList()
{
	return &list;
}

MasterServerInfo *MasterServer::GetServerInfo(int Index)
{
	if(Index >= (int)list.size()) return 0;
	return &list[Index];
}

int MasterServer::GetNumServers()
{
	return (int)list.size();
}

int MasterServer::GetServerPing(char *ip, int port)
{
	return port;
}

bool MasterServer::IsUserInfo()
{
	if(user->fmpid == 0) return 0;
	if(strlen(user->login) == 0) return 0;
	if(strlen(user->name) == 0) return 0;
	if(strlen(user->seskey) == 0) return 0;
	if(strlen(user->status) == 0) return 0;

	return 1;
}
MasterUserInfo *MasterServer::GetUserInfo()
{
	return user;
}
int MasterServer::GetUserId()
{
	return user->fmpid;
}
char *MasterServer::GetUserName()
{
	return user->name;
}