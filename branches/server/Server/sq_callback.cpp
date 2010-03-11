/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "sq\squirrel.h"
#include "sq\sqstdsystem.h"
#include "main.h"

void sc_OnGameModeInit(HSQUIRRELVM v)
{
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnGameModeInit"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_call(v,1,0,0); 
	}
	sq_settop(v,top); 
}

void sc_OnGameModeExit(HSQUIRRELVM v)
{
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnGameModeExit"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_call(v,1,0,0); 
	}
	sq_settop(v,top); 
}

void sc_OnFilterScriptInit(HSQUIRRELVM v)
{
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnFilterScriptInit"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_call(v,1,0,0); 
	}
	sq_settop(v,top); 
}

void sc_OnFilterScriptExit(HSQUIRRELVM v)
{
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnFilterScriptExit"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_call(v,1,0,0); 
	}
	sq_settop(v,top); 
}

int sc_OnPlayerConnect(HSQUIRRELVM v, int playerid, char name[32])
{
	int result;
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnPlayerConnect"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_pushinteger(v,playerid); 
		sq_pushstring(v,name,-1);
		sq_call(v,3,1,0); 
		sq_getinteger(v, sq_gettop(v), &result);
	}
	sq_settop(v,top);
	return result;
}

void sc_OnPlayerDisconnect(HSQUIRRELVM v, int playerid)
{
	int result;
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnPlayerConnect"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_pushinteger(v,playerid); 
		sq_call(v,2,0,0); 
	}
	sq_settop(v,top); 
}

void sc_OnPlayerSpawn(HSQUIRRELVM v, int playerid, int cl)
{
	int result;
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnPlayerSpawn"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_pushinteger(v,playerid); 
		sq_pushinteger(v,cl); 
		sq_call(v,3,0,0); 
	}
	sq_settop(v,top); 
}

void sc_CommandCallback(HSQUIRRELVM v, const char *callback, const unsigned char numargs)
{
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC(callback),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_pushinteger(v,numargs); 
		sq_call(v,2,0,0);
	}
	sq_settop(v,top);
}