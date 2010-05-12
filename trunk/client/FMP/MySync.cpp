/****************************************************************
Syncronizashion this player
(c) WNeZRoS 2009
*****************************************************************/



#include "log.h"
#include "main.h"
#include "Hook\classes.h"
#include "Hook\scripting.h"
#include "functions.h"
#include "..\..\Shared\Network\NetworkProtocol.h"

// RakNet
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "BitStream.h"

using namespace Natives;

extern RakPeerInterface *net;
extern short MyID;
extern bool myEnter;


void FMPHook::MoveSync()
{
	Log("Move Sync START");
	if(MyID < 0 || !DoesCharExist(gPlayer[MyID].PedID)) 
	{
		GetPlayerChar(_GetPlayer(), &gPlayer[MyID].PedID);
		return;
	}
	float x,y,z,a;
	GetCharCoordinates(gPlayer[MyID].PedID, &x, &y, &z);
	GetCharHeading(gPlayer[MyID].PedID, &a);
	if(gPlayer[MyID].position[0] != x && gPlayer[MyID].position[1] != y && gPlayer[MyID].position[2] != z && myEnter == 0)
	{
		float lx,ly,lz;
		GetCharCoordinates(gPlayer[MyID].PedID, &lx, &ly, &lz);
		float d = GetDist(lx, ly, lz, gPlayer[MyID].position[0], gPlayer[MyID].position[1], gPlayer[MyID].position[2]);
		float t = (GetTickCount()-gPlayer[MyID].last_active);
		float speed = (d / t)*10000;

		if(IsCharInAnyCar(gPlayer[MyID].PedID) && gPlayer[MyID].vehicleindex != -1) 
		{
			int pl_car = gPlayer[MyID].vehicleindex;
			Vehicle car = gCar[pl_car].CarID;

			float cs;
			GetCarSpeed(car, &cs);
			if(cs > speed) speed = cs;

			int dr = 1;
			if(!DoesVehicleExist(car)) { Debug("DOES"); }
			if(IsCarStopped(car)) { Debug("STOP"); dr = 0; }
			Vector3 v;
			GetCarSpeedVector(car, &v, 1);
			float x = floor(v.X * 1000 + 0.5)/1000;
			float y = floor(v.Y * 1000 + 0.5)/1000;
			if(abs(x) > abs(y))
			{
				// ��� �������� � (��������)
				if(x < 0)
					dr = -1;
			}
			else
			{
				// ��� �������� Y
				if(y < 0)
					dr = -1;
			}
			speed = speed * dr;
			GetCarHeading(car, &a);
		}
		NetworkPlayerMoveData data;
		data.position[0] = x;
		data.position[1] = y;
		data.position[2] = z;
		data.angle = a;
		data.speed = speed;
		RakNet::BitStream bsSend;
		bsSend.Write(data);
		net->RPC("RPC_MovePlayer",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		Log("MOVE SENDed");
		gPlayer[MyID].position[0] = x;
		gPlayer[MyID].position[1] = y;
		gPlayer[MyID].position[2] = z;
	}
	Log("MOVE");
	if(IsCharInAir(gPlayer[MyID].PedID))
	{
		RakNet::BitStream bsSend;
		net->RPC("RPC_JumpPlayer",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		Log("JUMP SENDed");
	}
	Log("AIR");
	if(IsCharDucking(gPlayer[MyID].PedID) != gPlayer[MyID].isducking)
	{
		gPlayer[MyID].isducking = IsCharDucking(gPlayer[MyID].PedID);
		NetworkPlayerDuckData data;
		data.shouldduck = gPlayer[MyID].isducking;
		RakNet::BitStream bsSend;
		bsSend.Write(data);
		net->RPC("RPC_DuckPlayer",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		Log("DUCK SENDed");
	}
}

void FMPHook::CarDoSync()
{
	if(IsCharInAnyCar(gPlayer[MyID].PedID) && gPlayer[MyID].vehicleindex == -1)
	{
		gPlayer[MyID].vehicleindex = GetPlayerCar(_GetPedVehicle(gPlayer[MyID].PedID));
		NetworkPlayerEntranceInVehicleData data;
		data.vehicleindex = gPlayer[MyID].vehicleindex;
		data.seat = 100;
		RakNet::BitStream bsSend;
		bsSend.Write(data);
		net->RPC("RPC_PlayerEnterInVehicle",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
	else if(!IsCharInAnyCar(gPlayer[MyID].PedID) && gPlayer[MyID].vehicleindex != -1)
	{
		gPlayer[MyID].vehicleindex = -1;
	}
	Debug("INCAR");
	if(myEnter == 0 && GetAsyncKeyState(70) != 0)
	{
		if(!IsCharInAnyCar(gPlayer[MyID].PedID))
		{
			int carid;
			float x, y, z;
			Log("PED");
			GetCharCoordinates(gPlayer[MyID].PedID, &x, &y, &z);
			Log("GETCOORD");
			carid = _GetClosestCar(x, y, z, 10);
			if(carid != -1)
			{
				NetworkPlayerEntranceInVehicleData data;
				data.vehicleindex = carid;
				data.seat = 0;
				RakNet::BitStream bsSend;
				bsSend.Write(data);
				net->RPC("RPC_PlayerEnterInVehicle",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
				myEnter = 1;
			}
		} 
		else 
		{
			myEnter = 0;
			RakNet::BitStream bsSend;
			net->RPC("RPC_PlayerExitFromVehicle",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	}
	else if(GetAsyncKeyState(71) != 0 && !IsCharInAnyCar(gPlayer[MyID].PedID) && myEnter == 0)
	{
		Log("GGGGG");
		int carid, seatid = 0; 
		unsigned int max;
		float x, y, z;
		Log("CARPED");
		GetCharCoordinates(gPlayer[MyID].PedID, &x, &y, &z);
		Log("GETCOORD");
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
				TaskEnterCarAsPassenger(gPlayer[MyID].PedID, gCar[carid].CarID, -1, seatid);
				NetworkPlayerEntranceInVehicleData data;
				data.vehicleindex = carid;
				data.seat = seatid;
				RakNet::BitStream bsSend;
				bsSend.Write(data);
				net->RPC("RPC_PlayerEnterInVehicle",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
			}
		}
	}
	else if(myEnter == 1)
	{
		int incar = IsCharInAnyCar(gPlayer[MyID].PedID);
		incar += IsCharSittingInAnyCar(gPlayer[MyID].PedID);
		incar = incar * IsCharOnFoot(gPlayer[MyID].PedID);
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
				net->RPC("RPC_PlayerCancelEnterInVehicle",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
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
	if(gun != gPlayer[MyID].currentweapon)
	{
		gPlayer[MyID].currentweapon = gun;
		NetworkPlayerWeaponChangeData data;
		data.weapon = gun;
		RakNet::BitStream bsSend;
		bsSend.Write(data);
		net->RPC("RPC_SwapGun",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
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
		dam.pid = -1; dam.hp = 0; dam.armor = 0;

		unsigned char i;
		unsigned int hp, ar;
		for(i=0; i<MAX_PLAYERS; i++)
		{
			if(gPlayer[i].connected == 1)
			{
				GetCharHealth(gPlayer[i].PedID, &hp);
				GetCharArmour(gPlayer[i].PedID, &ar);
				if(ar != gPlayer[i].armor || hp != gPlayer[i].health)
				{
					dam.pid = i;
					dam.hp = hp;
					dam.armor = ar;
					break;
				}
			}
		}

		NetworkPlayerFireData data;
		data.position[0] = x;
		data.position[1] = y;
		data.position[2] = z;
		data.weapon = gun;
		data.time = time;
		data.target = i;
		data.health = hp;
		data.armor = ar;
		RakNet::BitStream bsSend;
		bsSend.Write(data);
		net->RPC("RPC_PlayerFire",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
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
		NetworkPlayerAimData data;
		data.position[0] = x;
		data.position[1] = y;
		data.position[2] = z;
		data.weapon = gun;
		data.time = time;
		RakNet::BitStream bsSend;
		bsSend.Write(data);
		net->RPC("RPC_PlayerAim",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
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
	unsigned int hp, arm;
	GetCharHealth(gPlayer[MyID].PedID, &hp);
	GetCharArmour(gPlayer[MyID].PedID, &arm);
	if(hp != gPlayer[MyID].health || arm != gPlayer[MyID].armor)
	{
		gPlayer[MyID].health = hp;
		gPlayer[MyID].armor = arm;
		NetworkPlayerHealthAndArmorChangeData data;
		data.health = hp;
		data.armor = arm;
		RakNet::BitStream bsSend;
		bsSend.Write(data);
		net->RPC("RPC_PlayerParams",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}