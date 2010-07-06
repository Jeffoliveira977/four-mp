/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "../sq/squirrel.h"
#include "../sq/sqstdsystem.h"
#include "../sq/sqstdio.h"

#include "../ServerCore.h"
#include "../PlayerManager.h"
#include "../VehicleManager.h"
#include "../NetworkManager.h"
#include "../logging.h"

extern ServerCore server;
extern PlayerManager playm;
extern VehicleManager vm;
extern NetworkManager nm;

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
