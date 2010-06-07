/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "sq_callbacks.h"

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

bool sc_OnPlayerConnect(HSQUIRRELVM v, const short index, const wchar_t *name)
{
	int result = 1;
	int top = sq_gettop(v);
	sq_pushroottable(v);
	sq_pushstring(v, _SC("OnPlayerConnect"), -1);
	if (SQ_SUCCEEDED(sq_get(v, -2))) {
		sq_pushroottable(v);
		sq_pushinteger(v, index);
		sq_pushstring(v, name, -1);
		if (SQ_FAILED(sq_call(v, 3, 1, 0)))
		{
			return true;
		}
		sq_getinteger(v, sq_gettop(v), &result);
	}
	sq_settop(v,top);
	return result;
}

void sc_OnPlayerDisconnect(HSQUIRRELVM v, const short index)
{
	int result;
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnPlayerDisconnect"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_pushinteger(v,index); 
		sq_call(v,2,0,0); 
	}
	sq_settop(v,top); 
}

void sc_OnPlayerSpawn(HSQUIRRELVM v, const short playerindex)
{
	int result;
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnPlayerSpawn"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_pushinteger(v,playerindex); 
		sq_call(v,2,0,0); 
	}
	sq_settop(v,top); 
}

bool sc_OnPlayerText(HSQUIRRELVM v, const short playerindex, const wchar_t *text)
{
	int result = 1;
	int top = sq_gettop(v);
	sq_pushroottable(v);
	sq_pushstring(v, _SC("OnPlayerText"), -1);
	if(SQ_SUCCEEDED(sq_get(v, -2))) {
		sq_pushroottable(v);
		sq_pushinteger(v, playerindex);
		sq_pushstring(v, text, -1);
		sq_call(v, 3, 0, 0);
		sq_getinteger(v, sq_gettop(v), &result);
	}
	sq_settop(v,top);
	return result != 0;
}

void sc_CommandCallback(HSQUIRRELVM v, const wchar_t *callback, const unsigned char numargs)
{
	int top = sq_gettop(v);
	sq_pushroottable(v);
	sq_pushstring(v, callback, -1);
	if(SQ_SUCCEEDED(sq_get(v, -2))) {
		sq_pushroottable(v);
		sq_pushinteger(v, numargs);
		sq_call(v, 2, 0, 0);
	}
	sq_settop(v, top);
}