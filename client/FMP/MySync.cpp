/****************************************************************
Syncronizashion this player
(c) WNeZRoS 2009
*****************************************************************/

#include "log.h"
#include "main.h"
#include "Hook\classes.h"
#include "Hook\scripting.h"
#include "functions.h"

#include "NetworkManager.h"

extern NetworkManager nm;

extern short MyID;
extern bool myEnter;


void FMPHook::MoveSync()
{
	Log("Move Sync START");
	if(MyID < 0 || !Natives::DoesCharExist(gPlayer[MyID].PedID)) 
	{
		Natives::GetPlayerChar(_GetPlayer(), &gPlayer[MyID].PedID);
		return;
	}
	float x,y,z,a;
	Natives::GetCharCoordinates(gPlayer[MyID].PedID, &x, &y, &z);
	Natives::GetCharHeading(gPlayer[MyID].PedID, &a);
	if(gPlayer[MyID].position[0] != x && gPlayer[MyID].position[1] != y && gPlayer[MyID].position[2] != z && myEnter == 0)
	{
		float lx,ly,lz;
		Natives::GetCharCoordinates(gPlayer[MyID].PedID, &lx, &ly, &lz);
		float d = GetDist(lx, ly, lz, gPlayer[MyID].position[0], gPlayer[MyID].position[1], gPlayer[MyID].position[2]);
		float t = (GetTickCount()-gPlayer[MyID].last_active);
		float speed = (d / t)*10000;

		if(Natives::IsCharInAnyCar(gPlayer[MyID].PedID) && gPlayer[MyID].vehicleindex != -1) 
		{
			int pl_car = gPlayer[MyID].vehicleindex;
			Vehicle car = gCar[pl_car].CarID;

			float cs;
			Natives::GetCarSpeed(car, &cs);
			if(cs > speed) speed = cs;

			int dr = 1;
			if(!Natives::DoesVehicleExist(car)) { Debug("DOES"); }
			if(Natives::IsCarStopped(car)) { Debug("STOP"); dr = 0; }
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
		gPlayer[MyID].position[0] = x;
		gPlayer[MyID].position[1] = y;
		gPlayer[MyID].position[2] = z;
		gPlayer[MyID].angle = a;
		nm.SendPlayerMove(speed);
	}
	if(Natives::IsCharInAir(gPlayer[MyID].PedID))
	{
		nm.SendPlayerJump();
	}
	if(Natives::IsCharDucking(gPlayer[MyID].PedID) != gPlayer[MyID].isducking)
	{
		gPlayer[MyID].isducking = Natives::IsCharDucking(gPlayer[MyID].PedID);
		nm.SendPlayerDuck();
	}
}

void FMPHook::CarDoSync()
{
	if (Natives::IsCharInAnyCar(gPlayer[MyID].PedID) && gPlayer[MyID].vehicleindex == -1)
	{
		gPlayer[MyID].vehicleindex = GetPlayerCar(_GetPedVehicle(gPlayer[MyID].PedID));
		nm.SendPlayerEntranceInVehicle(0);
	}
	else if(!Natives::IsCharInAnyCar(gPlayer[MyID].PedID) && gPlayer[MyID].vehicleindex != -1)
	{
		gPlayer[MyID].vehicleindex = -1;
	}
	if(myEnter == 0 && GetAsyncKeyState(70) != 0)
	{
		if(!Natives::IsCharInAnyCar(gPlayer[MyID].PedID))
		{
			short carid;
			float x, y, z;
			Log("PED");
			Natives::GetCharCoordinates(gPlayer[MyID].PedID, &x, &y, &z);
			Log("GETCOORD");
			carid = _GetClosestCar(x, y, z, 10);
			if(carid != -1)
			{
				nm.SendPlayerEntranceInVehicle(0);
				myEnter = 1;
			}
		} 
		else 
		{
			myEnter = 0;
			nm.SendPlayerExitFromVehicle();
		}
	}
	else if(GetAsyncKeyState(71) != 0 && !Natives::IsCharInAnyCar(gPlayer[MyID].PedID) && myEnter == 0)
	{
		Log("GGGGG");
		short carid;
		char seatid = 0; 
		unsigned int max;
		float x, y, z;
		Log("CARPED");
		Natives::GetCharCoordinates(gPlayer[MyID].PedID, &x, &y, &z);
		Log("GETCOORD");
		carid = _GetClosestCar(x, y, z, 10);
		if(carid != -1)
		{
			Natives::GetMaximumNumberOfPassengers(gCar[carid].CarID, &max);
			if(max > 2)
			{
				float cx, cy, cz, al;
				Natives::GetCarCoordinates(gCar[carid].CarID, &cx, &cy, &cz);
				Natives::GetCarHeading(gCar[carid].CarID, &al);

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
				Natives::TaskEnterCarAsPassenger(gPlayer[MyID].PedID, gCar[carid].CarID, -1, seatid);
				nm.SendPlayerEntranceInVehicle(seatid);
			}
		}
	}
	else if(myEnter == 1)
	{
		int incar = Natives::IsCharInAnyCar(gPlayer[MyID].PedID);
		incar += Natives::IsCharSittingInAnyCar(gPlayer[MyID].PedID);
		incar = incar * Natives::IsCharOnFoot(gPlayer[MyID].PedID);
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
				nm.SendPlayerCancelEntranceInVehicle();
			}
		}
	}
}

void FMPHook::GunSync()
{
	Log("Start GunSync");
	eWeapon gun;
	Natives::GetCurrentCharWeapon(gPlayer[MyID].PedID, &gun);
	if(gun != gPlayer[MyID].currentweapon)
	{
		gPlayer[MyID].currentweapon = gun;
		nm.SendPlayerWeaponChange();
	}
	if(Natives::IsControlPressed(0, 137)!=0) // Fire
	{
		//FreezeCharPosition(gPlayer[MyID].PedID, 1); //Why the hell should we freeze him?
		gPlayer[MyID].isaiming = true;

		int time;
		float position[3];
		time = GetTickCount()-gPlayer[MyID].last_active;
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
		//FreezeCharPosition(gPlayer[MyID].PedID, 1); //Why the hell should we freeze him?
		gPlayer[MyID].isaiming = true;
		int time;
		float position[3];
		time = GetTickCount()-gPlayer[MyID].last_active;
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
	/*else if(gPlayer[MyID].isaiming)
	{
		if(gPlayer[MyID].edFreeze == 0)
			FreezeCharPosition(gPlayer[MyID].PedID, 0);
		gPlayer[MyID].Aim = 0;
	}*/
	Log("End GunSync");
}

void FMPHook::StatusSync()
{
	unsigned int hp, arm;
	Natives::GetCharHealth(gPlayer[MyID].PedID, &hp);
	Natives::GetCharArmour(gPlayer[MyID].PedID, &arm);
	if(hp != gPlayer[MyID].health || arm != gPlayer[MyID].armor)
	{
		gPlayer[MyID].health = hp;
		gPlayer[MyID].armor = arm;
		nm.SendPlayerHealthAndArmorChange();
	}
}