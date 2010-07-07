#include "sq_gamenatives.h"

#include "../ServerCore.h"
#include "../PlayerManager.h"
#include "../VehicleManager.h"
#include "../NetworkManager.h"
#include "../../Shared/logging/log.h"

extern ServerCore server;
extern PlayerManager playm;
extern VehicleManager vm;
extern NetworkManager nm;

void sq_GetGameTime(HSQUIRRELVM v)
{
	unsigned char time[2];
	server.GetGameTime(time);
	sq_newarray(v, 0);
	sq_pushinteger(v, time[0]);
	sq_arrayappend(v, -2);
	sq_pushinteger(v, time[1]);
	sq_arrayappend(v, -2);
	sq_push(v, -1);
}

void sq_SetGameTime(HSQUIRRELVM v)
{
	int h, m;
	sq_pushnull(v);
	sq_next(v, -2);
	sq_getinteger(v, -1, &h);
	sq_pop(v, 2);
	sq_next(v, -2);
	sq_getinteger(v, -1, &m);
	sq_pop(v, 2);
	sq_pop(v, 1);
	unsigned char time[2] = { h, m };
	sq_pushbool(v, server.SetGameTime(time));
}