/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "sq\squirrel.h"
#include "sq\sqstdsystem.h"

#include "net\RakNetworkFactory.h"
#include "net\RakPeerInterface.h"
#include "net\BitStream.h"

#include "main.h"
#include "manager.h"

extern RakPeerInterface *net;

void sq_CreateCar(HSQUIRRELVM v) 
{ 
	int car_id = 0;
	int model = 0;
	float x, y, z, rot;
	int color[2];

	sq_getinteger(v, 2, &model);
	sq_getfloat(v, 3, &x);
	sq_getfloat(v, 4, &y);
	sq_getfloat(v, 5, &z);
	sq_getfloat(v, 6, &rot);
	sq_getinteger(v, 7, &color[0]);
	sq_getinteger(v, 8, &color[1]);

	for(int i=0; i< MAX_VEHICLES;i++)
	{
		if(gVehicle[i].exist == 0)
		{
			car_id = i;
			break;
		}
	}

	con.Debug("%d = CreateCar(%d, %f, %f, %f, %f);", car_id, model, x, y, z, rot);

	gVehicle[car_id].model = model;
	gVehicle[car_id].x = x;
	gVehicle[car_id].y = y;
	gVehicle[car_id].z = z;
	gVehicle[car_id].angle = rot;
	gVehicle[car_id].color[0] = color[0];
	gVehicle[car_id].color[1] = color[1];
	gVehicle[car_id].exist = 1;
	gVehicle[car_id].engHealth = 1000;
	gVehicle[car_id].Health = 1000;
	gVehicle[car_id].DoorLock[0] = 0;
	gVehicle[car_id].DoorLock[1] = 0;
	gVehicle[car_id].DoorLock[2] = 0;
	gVehicle[car_id].DoorLock[3] = 0;
	gVehicle[car_id].DoorLock[4] = 0;
	gVehicle[car_id].DoorLock[5] = 0;
	
	RakNet::BitStream bsSend;
	bsSend.Write(car_id);
	bsSend.Write(model);
	bsSend.Write(x);
	bsSend.Write(y);
	bsSend.Write(z);
	bsSend.Write(rot);
	bsSend.Write(color[0]);
	bsSend.Write(color[1]);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(gPlayer[i].connected == 1)
		{
			net->RPC("CreateVehicle",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa ,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
	sq_pushinteger(v, car_id);
} 

void sq_GiveWeapon(HSQUIRRELVM v) 
{ 
	int gun = 0;
	int ammo = 0;
	int index;

	sq_getinteger(v, 2, &index);
	sq_getinteger(v, 3, &gun);
	sq_getinteger(v, 4, &ammo);

	gPlayer[index].gWeapons[0] = gun;
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
	}
} 

void sq_addPlayerClass(HSQUIRRELVM v) 
{ 
	int id = sConf.NumSkins;
	if(id == MAX_PCLASS) 
	{
		con.Print("Not free player class (MAX CLASSES = %d)", MAX_PCLASS);
		sq_pushinteger(v, -1);
	}

	sq_getinteger(v, 2, &pClass[id].model);
	sq_getfloat(v, 3, &pClass[id].x);
	sq_getfloat(v, 4, &pClass[id].y);
	sq_getfloat(v, 5, &pClass[id].z);
	sq_getfloat(v, 6, &pClass[id].angle);
	sq_getinteger(v, 7, &pClass[id].gun[0]);
	sq_getinteger(v, 8, &pClass[id].ammo[0]);
	sq_getinteger(v, 9, &pClass[id].gun[1]);
	sq_getinteger(v, 10, &pClass[id].ammo[1]);
	sq_getinteger(v, 11, &pClass[id].gun[2]);
	sq_getinteger(v, 12, &pClass[id].ammo[2]);

	sConf.NumSkins++;
	sq_pushinteger(v, id);
} 

void sq_enableComponentSelect(HSQUIRRELVM v) 
{ 
	sq_getbool(v, 2, (SQBool*)&sConf.ComponentSelect);
} 