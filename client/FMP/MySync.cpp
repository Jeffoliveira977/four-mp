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

void FMPHook::MoveSync()
{
	if(client.GetIndex() < 0 || !Natives::DoesCharExist(gPlayer[client.GetIndex()].PedID)) 
	{
		Natives::GetPlayerChar(_GetPlayer(), &gPlayer[client.GetIndex()].PedID);
		return;
	}
	float x,y,z,a;
	Natives::GetCharCoordinates(gPlayer[client.GetIndex()].PedID, &x, &y, &z);
	Natives::GetCharHeading(gPlayer[client.GetIndex()].PedID, &a);
	if (((gPlayer[client.GetIndex()].position[0] != x) || (gPlayer[client.GetIndex()].position[1] != y) || (gPlayer[client.GetIndex()].position[2] != z)) && (playerstate != PlayerStateEnteringVehicle))
	{
		float lx,ly,lz;
		Natives::GetCharCoordinates(gPlayer[client.GetIndex()].PedID, &lx, &ly, &lz);
		float d = GetDist(lx, ly, lz, gPlayer[client.GetIndex()].position[0], gPlayer[client.GetIndex()].position[1], gPlayer[client.GetIndex()].position[2]);
		float t = (GetTickCount()-gPlayer[client.GetIndex()].last_active);
		float speed = (d / t)*10000;

		if ((Natives::IsCharInAnyCar(gPlayer[client.GetIndex()].PedID) && (gPlayer[client.GetIndex()].vehicleindex != -1)))
		{
			int pl_car = gPlayer[client.GetIndex()].vehicleindex;
			Vehicle car = gVehicle[pl_car].CarID;

			float cs;
			Natives::GetCarSpeed(car, &cs);
			if(cs > speed) speed = cs;

			int dr = 1;
			if(!Natives::DoesVehicleExist(car)) { Log::Debug(L"DOES"); }
			//if(Natives::IsCarStopped(car)) { Log::Debug(L"STOP"); dr = 0; }
			Vector3 v;
			Natives::GetCarSpeedVector(car, &v, 1);
			float x = floor(v.X * 1000 + 0.5)/1000;
			float y = floor(v.Y * 1000 + 0.5)/1000;
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
			speed = speed * dr;
			Natives::GetCarHeading(car, &a);
		}
		gPlayer[client.GetIndex()].position[0] = x;
		gPlayer[client.GetIndex()].position[1] = y;
		gPlayer[client.GetIndex()].position[2] = z;
		gPlayer[client.GetIndex()].angle = a;
		nm.SendPlayerMove(speed);
	}
	if(Natives::IsCharInAir(gPlayer[client.GetIndex()].PedID))
	{
		nm.SendPlayerJump();
	}
	if(Natives::IsCharDucking(gPlayer[client.GetIndex()].PedID) != gPlayer[client.GetIndex()].isducking)
	{
		gPlayer[client.GetIndex()].isducking = Natives::IsCharDucking(gPlayer[client.GetIndex()].PedID);
		nm.SendPlayerDuck();
	}
}

void FMPHook::CarDoSync()
{
	bool enterpressed = false;
	if ((Natives::IsControlPressed(0, 3)) || (Natives::IsControlPressed(0, 43)))
	{
		//Log::Debug(L"Enter/exit vehicle pressed");
		enterpressed = true;
	}
	//bool getting = Natives::IsCharGettingInToACar(gPlayer[client.GetIndex()].PedID);
	bool invehicle = Natives::IsCharInAnyCar(gPlayer[client.GetIndex()].PedID);
	bool sitting = Natives::IsCharSittingInAnyCar(gPlayer[client.GetIndex()].PedID);
	bool onfoot = Natives::IsCharOnFoot(gPlayer[client.GetIndex()].PedID);
	//Log::Debug(L"In vehicle: %d. Sitting: %d. On foot: %d", invehicle, sitting, onfoot);
	switch (playerstate)
	{
	case PlayerStateOnFoot:
		{
			if ((!invehicle) && (!onfoot))
			{
				short vehicleindex;
				float x, y, z;
				Natives::GetCharCoordinates(gPlayer[client.GetIndex()].PedID, &x, &y, &z);
				vehicleindex = _GetClosestCar(x, y, z, 10);
				if (vehicleindex != INVALID_VEHICLE_INDEX)
				{
					playerstate = PlayerStateEnteringVehicle;
					gPlayer[client.GetIndex()].vehicleindex = vehicleindex;
					gPlayer[client.GetIndex()].seatindex = 0;
					nm.SendPlayerStartEntranceInVehicle();
				}
			}
			break;
		}
	case PlayerStateEnteringVehicle:
		{
			if ((invehicle) && (sitting))
			{
				playerstate = PlayerStateInVehicle;
				nm.SendPlayerFinishEntranceInVehicle();
			}
			else if (onfoot)
			{
				playerstate = PlayerStateOnFoot;
				nm.SendPlayerCancelEntranceInVehicle();
			}
			break;
		}
	case PlayerStateInVehicle:
		{
			if ((invehicle) && (!sitting))
			{
				playerstate = PlayerStateExitingVehicle;
				nm.SendPlayerStartExitFromVehicle();
			}
			break;
		}
	case PlayerStateExitingVehicle:
		{
			if (onfoot)
			{
				playerstate = PlayerStateOnFoot;
				nm.SendPlayerFinishExitFromVehicle();
			}
			break;
		}
	}
	/*if ( && gPlayer[client.GetIndex()].vehicleindex == -1)
	{
		gPlayer[client.GetIndex()].vehicleindex = GetPlayerCar(_GetPedVehicle(gPlayer[client.GetIndex()].PedID));
		nm.SendPlayerEntranceInVehicle(0);
	}*/
	/*else if(!Natives::IsCharInAnyCar(gPlayer[client.GetIndex()].PedID) && gPlayer[client.GetIndex()].vehicleindex != -1)
	{
		gPlayer[client.GetIndex()].vehicleindex = -1;
		nm.SendPlayerExitFromVehicle();
	}
	if ((myEnter == 0) && (GetAsyncKeyState(70) != 0) && (client.GetInputState() == InputStateGame))
	{
		if(!Natives::IsCharInAnyCar(gPlayer[client.GetIndex()].PedID))
		{
			
		} 
		else 
		{
			gPlayer[client.GetIndex()].vehicleindex = -1;
			gPlayer[client.GetIndex()].seatindex = -1;
			myEnter = 0;
			nm.SendPlayerExitFromVehicle();
		}
	}
	else if(GetAsyncKeyState(71) != 0 && !Natives::IsCharInAnyCar(gPlayer[client.GetIndex()].PedID) && myEnter == 0)
	{
		short carid;
		char seatid = 0; 
		unsigned int max;
		float x, y, z;
		Natives::GetCharCoordinates(gPlayer[client.GetIndex()].PedID, &x, &y, &z);

		carid = _GetClosestCar(x, y, z, 10);
		if(carid != -1)
		{
			Natives::GetMaximumNumberOfPassengers(gVehicle[carid].CarID, &max);
			if(max > 2)
			{
				float cx, cy, cz, al;
				Natives::GetCarCoordinates(gVehicle[carid].CarID, &cx, &cy, &cz);
				Natives::GetCarHeading(gVehicle[carid].CarID, &al);

				float sx,sy, br = 1000;
				int min = -1;
				for(char i=0; i<4; i++)
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
				gPlayer[client.GetIndex()].vehicleindex = carid;
				gPlayer[client.GetIndex()].seatindex = seatid+1;
				Natives::TaskEnterCarAsPassenger(gPlayer[client.GetIndex()].PedID, gVehicle[carid].CarID, -1, seatid);
				nm.SendPlayerEntranceInVehicle(seatid+1);
			}
		}
	}
	else if(myEnter == 1)
	{
		int incar = Natives::IsCharInAnyCar(gPlayer[client.GetIndex()].PedID);
		incar += Natives::IsCharSittingInAnyCar(gPlayer[client.GetIndex()].PedID);
		incar = incar * Natives::IsCharOnFoot(gPlayer[client.GetIndex()].PedID);
		if(incar == 0)
			myEnter = 0;
		else
		{
			int control = Natives::IsControlPressed(0, 2);
			control += Natives::IsControlPressed(0, 40);
			control += Natives::IsControlPressed(0, 41);
			control += Natives::IsControlPressed(0, 91);
			control += Natives::IsControlPressed(0, 144);

			if(control != 0)
			{
				myEnter = 0;
				gPlayer[client.GetIndex()].vehicleindex = -1;
				gPlayer[client.GetIndex()].seatindex = -1;
				nm.SendPlayerCancelEntranceInVehicle();
			}
		}
	}*/
}

void FMPHook::GunSync()
{
	eWeapon gun;
	Natives::GetCurrentCharWeapon(gPlayer[client.GetIndex()].PedID, &gun);
	if(gun != gPlayer[client.GetIndex()].currentweapon)
	{
		gPlayer[client.GetIndex()].currentweapon = gun;
		nm.SendPlayerWeaponChange();
	}
	if(Natives::IsControlPressed(0, 137)!=0) // Fire
	{
		//FreezeCharPosition(gPlayer[client.GetIndex()].PedID, 1); //Why the hell should we freeze him?
		gPlayer[client.GetIndex()].isaiming = true;

		int time;
		float position[3];
		time = GetTickCount()-gPlayer[client.GetIndex()].last_active;
		//GetCamTargetedCoords(&x, &y, &z);
		Camera cam;
		float cx, cy, cz;
		float ax, ay, az;
		float a, b, c;
		Natives::GetGameCam(&cam);
		Natives::GetCamPos(cam, &cx, &cy, &cz);
		Natives::GetCamRot(cam, &ax, &ay, &az);
		GetTarget(ax, az, &a, &b, &c, 5);
		position[0] = a + cx;
		position[1] = b + cy;
		position[2] = c + cz;

		short pid = -1;
		unsigned int hp, ar;
		for (short i = 0; i < MAX_PLAYERS; i++)
		{
			if(gPlayer[i].connected == 1)
			{
				Natives::GetCharHealth(gPlayer[i].PedID, &hp);
				Natives::GetCharArmour(gPlayer[i].PedID, &ar);
				if(ar != gPlayer[i].armor || hp != gPlayer[i].health)
				{
					pid = i;
					break;
				}
			}
		}
		nm.SendPlayerFire(position, time, pid, hp, ar);		
	}
	else if(Natives::IsControlPressed(0, 138)!=0) // Aim
	{
		//FreezeCharPosition(gPlayer[client.GetIndex()].PedID, 1); //Why the hell should we freeze him?
		gPlayer[client.GetIndex()].isaiming = true;
		int time;
		float position[3];
		time = GetTickCount()-gPlayer[client.GetIndex()].last_active;
		//GetCamTargetedCoords(&x, &y, &z);
		Camera cam;
		float cx, cy, cz;
		float ax, ay, az;
		float a, b, c;
		Natives::GetGameCam(&cam);
		Natives::GetCamPos(cam, &cx, &cy, &cz);
		Natives::GetCamRot(cam, &ax, &ay, &az);
		GetTarget(ax, az, &a, &b, &c, 5);
		position[0] = a + cx;
		position[1] = b + cy;
		position[2] = c + cz;
		nm.SendPlayerAim(position, time);
	}
	/*else if(gPlayer[client.GetIndex()].isaiming)
	{
		if(gPlayer[client.GetIndex()].edFreeze == 0)
			FreezeCharPosition(gPlayer[client.GetIndex()].PedID, 0);
		gPlayer[client.GetIndex()].Aim = 0;
	}*/
}

void FMPHook::StatusSync()
{
	unsigned int hp, arm;
	Natives::GetCharHealth(gPlayer[client.GetIndex()].PedID, &hp);
	Natives::GetCharArmour(gPlayer[client.GetIndex()].PedID, &arm);
	if(hp != gPlayer[client.GetIndex()].health || arm != gPlayer[client.GetIndex()].armor)
	{
		gPlayer[client.GetIndex()].health = hp;
		gPlayer[client.GetIndex()].armor = arm;
		nm.SendPlayerHealthAndArmorChange();
	}
}