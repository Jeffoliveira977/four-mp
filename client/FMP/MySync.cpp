/****************************************************************
Syncronizashion this player
(c) WNeZRoS 2009
*****************************************************************/

#include <math.h>

#include "log.h"
#include "../Shared/ClientCore.h"
#include "Hook\classes.h"
#include "Hook\scripting.h"
#include "functions.h"

#include "PlayerManager.h"
#include "VehicleManager.h"
#include "../Shared/NetworkManager.h"

extern ClientCore client;
extern NetworkManager nm;

extern FPlayer gPlayer[MAX_PLAYERS];
extern FVehicle gVehicle[MAX_VEHICLES];

void FMPHook::FootSync()
{
	if(client.GetIndex() < 0 || !Natives::DoesCharExist(gPlayer[client.GetIndex()].PedID)) 
	{
		Natives::GetPlayerChar(_GetPlayer(), &gPlayer[client.GetIndex()].PedID);
		return;
	}
	// write info in our temp sync struct
	Natives::GetCharCoordinates(gPlayer[client.GetIndex()].PedID, &own_foot_sync.position[0],&own_foot_sync.position[1],&own_foot_sync.position[2]);
	own_foot_sync.position[2] -= 1.0; // z correction
	Natives::GetCharVelocity(gPlayer[client.GetIndex()].PedID, &own_foot_sync.velocity[0],&own_foot_sync.velocity[1],&own_foot_sync.velocity[2]);
	Natives::GetCharHeading(gPlayer[client.GetIndex()].PedID, &own_foot_sync.angle);
	Natives::GetCharHealth(gPlayer[client.GetIndex()].PedID, &own_foot_sync.health);
	Natives::GetCharArmour(gPlayer[client.GetIndex()].PedID, &own_foot_sync.armour);
	Natives::GetCharSpeed(gPlayer[client.GetIndex()].PedID, &own_foot_sync.speed);
	Natives::GetCurrentCharWeapon(gPlayer[client.GetIndex()].PedID, &own_foot_sync.weapon);
	//Natives::GetAmmoInCharWeapon(gPlayer[client.GetIndex()].PedID, own_foot_sync.weapon, &own_foot_sync.ammo);
	if(Natives::IsCharDucking(gPlayer[client.GetIndex()].PedID)) own_foot_sync.is_dunk = 1;
	else own_foot_sync.is_dunk = 0;
	if(Natives::IsCharInAir(gPlayer[client.GetIndex()].PedID)) own_foot_sync.is_jump = 1;
	else own_foot_sync.is_jump = 0;
	if(!Natives::IsControlPressed(0, 137)) // Fire
	{
		own_foot_sync.shot_time = GetTickCount() - gPlayer[client.GetIndex()].last_active;

		Natives::GetGameCam(&t_cam);
		Natives::GetCamRot(t_cam, &t_cam_position[0],&t_cam_position[1],&t_cam_position[2]);
		GetTarget(t_cam_position[0],t_cam_position[2], &t_cam_position[3], &t_cam_position[4], &t_cam_position[5], 5);
		Natives::GetCamPos(t_cam, &t_cam_position[0],&t_cam_position[1],&t_cam_position[2]);
		t_cam_position[0] += t_cam_position[3];
		t_cam_position[1] += t_cam_position[4];
		t_cam_position[2] += t_cam_position[5];

		own_foot_sync.aim_sync = 2; // fire id
	}
	else if(!Natives::IsControlPressed(0, 138)) // Aim
	{
		own_foot_sync.shot_time = GetTickCount() - gPlayer[client.GetIndex()].last_active;

		Natives::GetGameCam(&t_cam);
		Natives::GetCamRot(t_cam, &t_cam_position[0],&t_cam_position[1],&t_cam_position[2]);
		GetTarget(t_cam_position[0],t_cam_position[2], &t_cam_position[3], &t_cam_position[4], &t_cam_position[5], 5);
		Natives::GetCamPos(t_cam, &t_cam_position[0],&t_cam_position[1],&t_cam_position[2]);
		t_cam_position[0] += t_cam_position[3];
		t_cam_position[1] += t_cam_position[4];
		t_cam_position[2] += t_cam_position[5];

		own_foot_sync.aim_sync = 1; // aim id
	}
	else own_foot_sync.aim_sync = 0;
	// send sync to server
	nm.SendPlayerFootSync(own_foot_sync);
}

void FMPHook::VehicleSync()
{
	if(client.GetIndex() < 0 || !Natives::DoesCharExist(gPlayer[client.GetIndex()].PedID)) 
	{
		Natives::GetPlayerChar(_GetPlayer(), &gPlayer[client.GetIndex()].PedID);
		return;
	}
	// write info in our temp sync struct
	Natives::GetCarCharIsUsing(gPlayer[client.GetIndex()].PedID, &t_car);
	own_veh_sync.v_id = GetPlayerCar(t_car);
	Natives::GetCarCoordinates(t_car, &own_veh_sync.position[0],&own_veh_sync.position[1],&own_veh_sync.position[2]);
	Natives::GetCarSpeedVector(t_car,&t_vec3,false);
	own_veh_sync.velocity[0] = t_vec3.X;
	own_veh_sync.velocity[1] = t_vec3.Y;
	own_veh_sync.velocity[2] = t_vec3.Z;
	Natives::GetCarSpeed(t_car,&own_veh_sync.speed);
	Natives::GetCarHeading(t_car, &own_veh_sync.angle);
	Natives::GetCarHealth(t_car, &own_veh_sync.v_health);
	Natives::GetCharHealth(gPlayer[client.GetIndex()].PedID, &own_veh_sync.health);
	Natives::GetCharArmour(gPlayer[client.GetIndex()].PedID, &own_veh_sync.armour);
	// send sync to server
	nm.SendPlayerVehicleSync(own_veh_sync);
}