/****************************************************************
Syncronizashion this player
(c) WNeZRoS 2009
*****************************************************************/



#include "log.h"
#include "main.h"
#include "Hook\classes.h"
//#include "Hook\hook.h"
#include "Hook\scripting.h"
#include "functions.h"

// RakNet
#include "net\RakNetworkFactory.h"
#include "net\RakPeerInterface.h"
#include "net\BitStream.h"

using namespace Natives;

extern RakPeerInterface *net;
extern bool myEnter;


void FMPHook::MoveSync()
{
	float x,y,z,a;
	GetCharCoordinates(gPlayer[MyID].PedID, &x, &y, &z);
	GetCharHeading(_GetPlayerPed(), &a);
	if(gPlayer[MyID].x != x && gPlayer[MyID].y != y && gPlayer[MyID].z != z && myEnter == 0)
	{
		Debug("MOVE SEND START");
		float lx,ly,lz;
		GetCharCoordinates(gPlayer[MyID].PedID, &lx, &ly, &lz);
		float d = GetDist(lx, ly, lz, gPlayer[MyID].x, gPlayer[MyID].y, gPlayer[MyID].z);
		float t = (GetTickCount()-gPlayer[MyID].last_active);
		float speed = (d / t)*10000;
		Debug("MOVE MATH");

		if(IsCharInAnyCar(_GetPlayerPed()) && gPlayer[MyID].car_id != -1) 
		{
			Debug("INCAR");
			Debug("%d, %f, %d", gPlayer[MyID].car_id, speed, MyID);
			int pl_car = gPlayer[MyID].car_id;
			Debug("lp_car %d", pl_car);
			Vehicle car = gCar[pl_car].CarID;
			Debug("car %d", car);

			float cs;
			GetCarSpeed(car, &cs);
			if(cs > speed) speed = cs;

			int dr = 1;
			Debug("CraDrive %d", car);
			if(!DoesVehicleExist(car)) { Debug("DOES"); }
			Debug("CarDrive x%dx", 1);
			if(IsCarStopped(car)) { Debug("STOP"); dr = 0; }
			Vector3 v;
			Debug("CarDrive x%dx", 2);
			GetCarSpeedVector(car, &v, 1);
			Debug("CarDrive x%dx", 3);
			float x = floor(v.X * 1000 + 0.5)/1000;
			float y = floor(v.Y * 1000 + 0.5)/1000;
			Debug("CarDrive x%dx - %fx%f", x, y);
			if(abs(x) > abs(y))
			{
				// ось движения Х (основная)
				if(x < 0)
					dr = -1;
			}
			else
			{
				// ось движения Y
				if(y < 0)
					dr = -1;
			}
			Debug("dr %d", dr);
			speed = speed * dr;
			GetCarHeading(car, &a);
		}
		Debug("ISCHARINCAR");
		RakNet::BitStream bsSend;
		bsSend.Write(x);
		bsSend.Write(y);
		bsSend.Write(z);
		bsSend.Write(a);
		bsSend.Write(speed);
		net->RPC("MovePlayer",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		Log("MOVE SENDed");
		gPlayer[MyID].x = x;
		gPlayer[MyID].y = y;
		gPlayer[MyID].z = z;
	}
	Debug("MOVE");
	if(IsCharInAir(gPlayer[MyID].PedID))
	{
		RakNet::BitStream bsSend;
		net->RPC("JumpPlayer",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		Log("JUMP SENDed");
	}
	Debug("AIR");
	if(IsCharDucking(gPlayer[MyID].PedID) != gPlayer[MyID].duck)
	{
		gPlayer[MyID].duck = IsCharDucking(gPlayer[MyID].PedID);
		RakNet::BitStream bsSend;
		bsSend.Write(gPlayer[MyID].duck);
		net->RPC("DuckPlayer",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		Log("DUCK SENDed");
	}
}

void FMPHook::CarDoSync()
{
	if(IsCharInAnyCar(_GetPlayerPed()) && gPlayer[MyID].car_id == -1)
	{
		gPlayer[MyID].car_id = GetPlayerCar(_GetPedVehicle(_GetPlayerPed()));
		RakNet::BitStream bsSend;
		bsSend.Write(gPlayer[MyID].car_id);
		bsSend.Write(100);
		net->RPC("PlayerEnterInVehicle",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
	else if(!IsCharInAnyCar(_GetPlayerPed()) && gPlayer[MyID].car_id != -1)
	{
		gPlayer[MyID].car_id = -1;
	}
	Debug("INCAR");
	if(myEnter == 0 && GetAsyncKeyState(70) != 0)
	{
		if(!IsCharInAnyCar(_GetPlayerPed()))
		{
			int carid;
			float x, y, z;
			GetCharCoordinates(_GetPlayerPed(), &x, &y, &z);
			carid = _GetClosestCar(x, y, z, 10);
			if(carid != -1)
			{
				RakNet::BitStream bsSend;
				bsSend.Write(carid);
				bsSend.Write(-1);
				net->RPC("PlayerEnterInVehicle",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
				myEnter = 1;
			}
		} 
		else 
		{
			myEnter = 0;
			RakNet::BitStream bsSend;
			net->RPC("PlayerExitFromVehicle",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	}
	else if(GetAsyncKeyState(71) != 0 && !IsCharInAnyCar(_GetPlayerPed()) && myEnter == 0)
	{
		Log("GGGGG");
		int carid, seatid = 0, max;
		float x, y, z;
		GetCharCoordinates(_GetPlayerPed(), &x, &y, &z);
		carid = _GetClosestCar(x, y, z, 10);
		if(carid != -1)
		{
			GetMaximumNumberOfPassengers(gCar[carid].CarID, &max);
			if(max > 2)
			{
				float cx, cy, cz, al;
				GetCarCoordinates(gCar[carid].CarID, &cx, &cy, &cz);
				GetCarHeading(gCar[carid].CarID, &al);

				float sx,sy, br = 1000;
				int min = -1;
				for(int i=0; i<4; i++)
				{
					if(i != 2)
					{
						float r, angle = 45+(90*i)+al;
						if(angle > 360) angle -=360;
						angle -= 180;
						GetTarget(0, angle, &sx, &sy, &z, 0.5f);
						sx+=cx;
						sy+=cy;
						r = GetDist(x, y, 0, sx, sy, 0);
						if(r < br)
						{
							br = r;
							min = i;
						}
					}
				}
				if(min == 0) seatid = 2;
				else if(min == 1) seatid = 0;
				else if(min == 3) seatid = 1;
			}
			if(seatid >= 0)
			{
				TaskEnterCarAsPassenger(_GetPlayerPed(), gCar[carid].CarID, -1, seatid);
				RakNet::BitStream bsSend;
				bsSend.Write(carid);
				bsSend.Write(seatid);
				net->RPC("PlayerEnterInVehicle",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
			}
		}
	}
	else if(myEnter == 1)
	{
		int incar = IsCharInAnyCar(_GetPlayerPed());
		incar += IsCharSittingInAnyCar(_GetPlayerPed());
		incar = incar * IsCharOnFoot(_GetPlayerPed());
		if(incar == 0)
			myEnter = 0;
		else
		{
			int control = IsControlPressed(0, 2);
			control += IsControlPressed(0, 40);
			control += IsControlPressed(0, 41);
			control += IsControlPressed(0, 91);
			control += IsControlPressed(0, 144);

			if(control != 0)
			{
				myEnter = 0;
				RakNet::BitStream bsSend;
				net->RPC("PlayerCancelEnterInVehicle",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
			}
		}
	}
	Debug("MYENTER");
}

void FMPHook::GunSync()
{
	Log("Start");
	int gun;
	GetCurrentCharWeapon(gPlayer[MyID].PedID, (eWeapon*)&gun);
	Log("GetGUN");
	if(gun != gPlayer[MyID].gun)
	{
		gPlayer[MyID].gun = gun;
		RakNet::BitStream bsSend;
		bsSend.Write(gun);
		net->RPC("SwapGun",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
	Log("AIM FIRE");
	if(IsControlPressed(0, 137)!=0) // Fire
	{
		FreezeCharPosition(gPlayer[MyID].PedID, 1);
		gPlayer[MyID].Aim = 1;

		int time;
		float x, y, z;
		time = GetTickCount()-gPlayer[MyID].last_active;
		//GetCamTargetedCoords(&x, &y, &z);
		Log("TARGET %d", 0);
		Camera cam;
		float cx, cy, cz;
		float ax, ay, az;
		float a, b, c;
		Log("TARGET %d", 1);
		GetGameCam(&cam);
		Log("TARGET %d", 11);
		GetCamPos(cam, &cx, &cy, &cz);
		Log("TARGET %d", 12);
		GetCamRot(cam, &ax, &ay, &az);
		Log("TARGET %d", 2);
		GetTarget(ax, az, &a, &b, &c, 5);
		Log("TARGET %d", 3);
		x = a + cx;
		y = b + cy;
		z = c + cz;

		PlayerDamage dam;
		dam.pid = -1; dam.hp = 0; dam.armour = 0;

		int hp, ar;
		for(int i=0; i<MAX_PLAYERS; i++)
		{
			if(gPlayer[i].connected == 1)
			{
				GetCharHealth(gPlayer[i].PedID, &hp);
				GetCharArmour(gPlayer[i].PedID, &ar);
				if(ar != gPlayer[i].armour || hp != gPlayer[i].health)
				{
					dam.pid = i;
					dam.hp = hp;
					dam.armour = ar;
					break;
				}
			}
		}

		RakNet::BitStream bsSend;
		bsSend.Write(x);
		bsSend.Write(y);
		bsSend.Write(z);
		bsSend.Write(gun);
		bsSend.Write(time);
		bsSend.Write(dam);
		net->RPC("PlayerFire",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
	else if(IsControlPressed(0, 138)!=0) // Aim
	{
		Log("Freeze");
		FreezeCharPosition(gPlayer[MyID].PedID, 1);
		gPlayer[MyID].Aim = 1;
		Log("Find");
		int time;
		float x, y, z;
		time = GetTickCount()-gPlayer[MyID].last_active;
		Log("Target");
		//GetCamTargetedCoords(&x, &y, &z);
		Log("TARGET %d", 0);
		Camera cam;
		float cx, cy, cz;
		float ax, ay, az;
		float a, b, c;
		Log("TARGET %d", 1);
		GetGameCam(&cam);
		Log("TARGET %d", 11);
		GetCamPos(cam, &cx, &cy, &cz);
		Log("TARGET %d", 12);
		GetCamRot(cam, &ax, &ay, &az);
		Log("TARGET %d", 2);
		GetTarget(ax, az, &a, &b, &c, 5);
		Log("TARGET %d", 3);
		x = a + cx;
		y = b + cy;
		z = c + cz;
		Log("TARGET %d", 4);
		Log("Send");
		RakNet::BitStream bsSend;
		bsSend.Write(x);
		bsSend.Write(y);
		bsSend.Write(z);
		bsSend.Write(gun);
		bsSend.Write(time);
		net->RPC("PlayerAim",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
	else if(gPlayer[MyID].Aim == 1)
	{
		if(gPlayer[MyID].edFreeze == 0)
			FreezeCharPosition(gPlayer[MyID].PedID, 0);
		gPlayer[MyID].Aim = 0;
	}
	Log("End");
}

void FMPHook::StatusSync()
{
	int hp, arm;
	GetCharHealth(_GetPlayerPed(), &hp);
	GetCharArmour(_GetPlayerPed(), &arm);
	if(hp != gPlayer[MyID].health || arm != gPlayer[MyID].armour)
	{
		gPlayer[MyID].health = hp;
		gPlayer[MyID].armour = arm;

		if(hp <= 100)
		{
			FadeScreenIn(100);
		}

		RakNet::BitStream bsSend;
		bsSend.Write(hp);
		bsSend.Write(arm);
		net->RPC("PlayerParams",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}