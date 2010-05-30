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

void sq_GetPlayerHealth(HSQUIRRELVM v)
{
	int index;
	sq_getinteger(v, 2, &index);
	int health;
	if (!playm.GetPlayerHealth(index, health))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushinteger(v, health);
}

void sq_GetPlayerArmor(HSQUIRRELVM v)
{
	int index;
	sq_getinteger(v, 2, &index);
	int armor;
	if (!playm.GetPlayerArmor(index, armor))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushinteger(v, armor);
}

void sq_GetPlayerWantedLevel(HSQUIRRELVM v)
{
	int index;
	sq_getinteger(v, 2, &index);
	char wantedlevel = playm.GetPlayerWantedLevel(index);
	sq_pushinteger(v, wantedlevel);
}

void sq_SetPlayerSpawnPos(HSQUIRRELVM v)
{
	int index = 0;
	float pos[4];
	sq_getinteger(v, 2, &index);
	sq_getfloat(v, 3, &pos[0]);
	sq_getfloat(v, 4, &pos[1]);
	sq_getfloat(v, 5, &pos[2]);
	sq_getfloat(v, 6, &pos[3]);

	playm.SetPlayerSpawnPos(index, pos);
}