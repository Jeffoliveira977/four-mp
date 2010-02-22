#pragma once

#define FCVAR_NONE				0
#define FCVAR_UNREGISTERED		(1<<0)	// If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY	(1<<1)	// Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL			(1<<2)	// defined by the game DLL
#define FCVAR_CLIENTDLL			(1<<3)  // defined by the client DLL
#define FCVAR_HIDDEN			(1<<4)	// Hidden. Doesn't appear in find or autocomplete. Like DEVELOPMENTONLY, but can't be compiled out.
#define FCVAR_PROTECTED			(1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY			(1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define	FCVAR_ARCHIVE			(1<<7)	// set to cause it to be saved to vars.rc
#define	FCVAR_NOTIFY			(1<<8)	// notifies players when changed
#define	FCVAR_USERINFO			(1<<9)	// changes the client's info string
#define FCVAR_PRINTABLEONLY		(1<<10)  // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_UNLOGGED			(1<<11)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING	(1<<12)  // never try to print that cvar
#define FCVAR_SERVER			(1<<13)	// server setting enforced on clients
#define FCVAR_CHEAT				(1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats
#define FCVAR_PLUGIN			(1<<15)	// Defined by a 3rd party plugin.
#define FCVAR_NOT_CONNECTED		(1<<16)	// cvar cannot be changed by a client that is connected to a server
#define FCVAR_SERVER_CAN_EXECUTE	(1<<17)// the server is allowed to execute this command on clients
#define FCVAR_SERVER_CANNOT_QUERY	(1<<18)// If this is set, then the server is not allowed to query this cvar's value
#define FCVAR_CLIENTCMD_CAN_EXECUTE	(1<<19)	//

class ConSymbolBase
{
public:
	ConSymbolBase(void);
	~ConSymbolBase(void);
	char *GetName(void);
	char *GetDescription(void);
	int GetFlags(void);
	void SetFlags(int f);
protected:
	char *name;
	char *description;
	int flags;
	void Init(const char *symbolname, const char *desc = "", const int symbolflags = 0);
	void Uninit(void);
};
