/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "sq\squirrel.h"
#include "sq\sqstdsystem.h"

#include "ServerCore.h"
#include "PlayerManager.h"
#include "VehicleManager.h"

extern ServerCore server;
extern PlayerManager playm;
extern VehicleManager vm;

void sq_CreateCar(HSQUIRRELVM v) 
{ 
	unsigned char index = 0;
	int model = 0;
	float position[3], angle;
	int color[2];

	sq_getinteger(v, 2, &model);
	sq_getfloat(v, 3, &position[0]);
	sq_getfloat(v, 4, &position[1]);
	sq_getfloat(v, 5, &position[2]);
	sq_getfloat(v, 6, &angle);
	sq_getinteger(v, 7, &color[0]);
	sq_getinteger(v, 8, &color[1]);
	if (!vm.CreateVehicle(model, position, angle, color, index))
	{
		sq_pushnull(v);
		return;
	}
	sq_pushinteger(v, index);
} 

void sq_GiveWeapon(HSQUIRRELVM v) 
{ 
	int gun = 0;
	int ammo = 0;
	int index;

	sq_getinteger(v, 2, &index);
	sq_getinteger(v, 3, &gun);
	sq_getinteger(v, 4, &ammo);

	/*gPlayer[index].gWeapons[0] = gun;
	gPlayer[index].gAmmo[0] = ammo;
	
	if(gPlayer[index].connected == 0) return;

	RakNet::BitStream bsSend;
	bsSend.Write(index);
	bsSend.Write(gun);
	bsSend.Write(ammo);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(gPlayer[i].connected == 1)
		{
			net->RPC("GiveGun",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa ,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}*/
} 

void sq_addPlayerClass(HSQUIRRELVM v) 
{ 
	int model;
	float position[3];
	float angle;
	int weapons[8];
	int ammo[8];
	unsigned char index;
	sq_getinteger(v, 2, &model);
	sq_getfloat(v, 3, &position[0]);
	sq_getfloat(v, 4, &position[1]);
	sq_getfloat(v, 5, &position[2]);
	sq_getfloat(v, 6, &angle);
	//TODO: Add support for all 8 weapon slots.
	sq_getinteger(v, 7, &weapons[0]);
	sq_getinteger(v, 8, &ammo[0]);
	sq_getinteger(v, 9, &weapons[1]);
	sq_getinteger(v, 10, &ammo[1]);
	sq_getinteger(v, 11, &weapons[2]);
	sq_getinteger(v, 12, &ammo[2]);
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