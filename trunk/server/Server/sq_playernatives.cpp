#include <stddef.h>

#include "sq_playernatives.h"
#include "PlayerManager.h"

extern PlayerManager playm;

void sq_GetPlayerName(HSQUIRRELVM v)
{
	int index;
	sq_getinteger(v, 2, &index);
	char *name = playm.GetPlayerName(index);
	if (name == NULL)
	{
		sq_pushnull(v);
		return;
	}
	sq_pushstring(v, name, -1);
}

void sq_GetPlayerModel(HSQUIRRELVM v)
{
	int index;
	sq_getinteger(v, 2, &index);
	int model = playm.GetPlayerModel(index);
	sq_pushinteger(v, model);
}

void sq_GetPlayerAngle(HSQUIRRELVM v)
{
	int index;
	sq_getinteger(v, 2, &index);
	float angle;
	if (!playm.GetPlayerAngle(index, angle))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushfloat(v, angle);
}

void sq_GetPlayerVehicle(HSQUIRRELVM v)
{
	int index;
	sq_getinteger(v, 2, &index);
	short vehicleindex = playm.GetPlayerVehicle(index);
	sq_pushinteger(v, vehicleindex);
}

void sq_GetPlayerScore(HSQUIRRELVM v)
{
	int index;
	sq_getinteger(v, 2, &index);
	int score;
	if (!playm.GetPlayerScore(index, score))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushinteger(v, score);
}

void sq_GetPlayerWantedLevel(HSQUIRRELVM v)
{
	int index;
	sq_getinteger(v, 2, &index);
	char wantedlevel = playm.GetPlayerWantedLevel(index);
	sq_pushinteger(v, wantedlevel);
}