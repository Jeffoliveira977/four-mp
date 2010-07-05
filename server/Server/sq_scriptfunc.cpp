/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "sq/squirrel.h"
#include "sq/sqstdsystem.h"
#include "sq/sqstdio.h"

#include "ServerCore.h"
#include "PlayerManager.h"
#include "VehicleManager.h"
#include "NetworkManager.h"
#include "logging.h"

extern ServerCore server;
extern PlayerManager playm;
extern VehicleManager vm;
extern NetworkManager nm;

void sq_CreateCar(HSQUIRRELVM v) 
{ 
	int model = 0;
	float position[3], angle;
	int tempcolor[2];
	
	sq_getinteger(v, 2, &model);
	sq_getfloat(v, 3, &position[0]);
	sq_getfloat(v, 4, &position[1]);
	sq_getfloat(v, 5, &position[2]);
	sq_getfloat(v, 6, &angle);
	sq_getinteger(v, 7, &tempcolor[0]);
	sq_getinteger(v, 8, &tempcolor[1]);
	unsigned char color[] = {tempcolor[0], tempcolor[1]};
	short index = vm.CreateVehicle(model, position, angle, color);
	if (index == INVALID_VEHICLE_INDEX)
	{
		sq_pushnull(v);
		return;
	}
	sq_pushinteger(v, index);
}

void sq_addPlayerClass(HSQUIRRELVM v) 
{
	int model;
	float position[3];
	float angle;
	int tempweapons[8];
	int tempammo[8];
	unsigned char index;
	sq_getinteger(v, 2, &model);
	sq_getfloat(v, 3, &position[0]);
	sq_getfloat(v, 4, &position[1]);
	sq_getfloat(v, 5, &position[2]);
	sq_getfloat(v, 6, &angle);
	//TODO: Add support for all 8 weapon slots.
	sq_getinteger(v, 7, &tempweapons[0]);
	sq_getinteger(v, 8, &tempammo[0]);
	sq_getinteger(v, 9, &tempweapons[1]);
	sq_getinteger(v, 10, &tempammo[1]);
	sq_getinteger(v, 11, &tempweapons[2]);
	sq_getinteger(v, 12, &tempammo[2]);
	char weapons[8];
	short ammo[8];
	for (unsigned char i = 0; i < 8; i++)
	{
		weapons[i] = tempweapons[i];
		ammo[i] = tempammo[i];
	}
	if (!playm.AddPlayerClass(model, position, angle, weapons, ammo, index))
	{
		sq_pushinteger(v, -1);
		return;
	}
	sq_pushinteger(v, index);
}

void sq_enableComponentSelect(HSQUIRRELVM v) 
{
	unsigned int enable;
	sq_getbool(v, 2, &enable);
	server.EnableComponentSelect(enable);
}

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

void sq_SetCmdCharFirst(HSQUIRRELVM v)
{
	wchar_t ch;
	sq_getinteger(v, 2, (SQInteger*)&ch);
	server.SetCmdChar(0, ch);
}

void sq_SetCmdCharSecond(HSQUIRRELVM v)
{
	wchar_t ch;
	sq_getinteger(v, 2, (SQInteger*)&ch);
	server.SetCmdChar(1, ch);
}

void sq_SendMessageToAll(HSQUIRRELVM v)
{
	wchar_t msg[255];
	sq_getstring(v, 2, (const SQChar**)&msg);
	nm.SendChatMessageToAll(msg);
}

void sq_SendMessageToPlayer(HSQUIRRELVM v)
{
	wchar_t msg[255];
	short index;
	sq_getinteger(v, 2, (SQInteger*)&index);
	sq_getstring(v, 3, (const SQChar**)&msg);
	nm.SendChatMessageToOne(index, msg);
}

void sq_Kick(HSQUIRRELVM v)
{
	short index;
	sq_getinteger(v, 2, (SQInteger*)&index);
	nm.KickPlayer(index);
}

void sq_Ban(HSQUIRRELVM v)
{
	short index;
	sq_getinteger(v, 2, (SQInteger*)&index);
	nm.BanPlayer(index);
}

void sq_ReloadBanList(HSQUIRRELVM v)
{
	nm.ReloadBanList();
}

void sq_ClearBanList(HSQUIRRELVM v)
{
	nm.ClearBanList();
}

void sq_include(HSQUIRRELVM v)
{
	SQChar filename[255];
	sq_getstring(v, 2, (const SQChar**)&filename);
	sqstd_dofile(v, filename, 0, SQTrue);
}