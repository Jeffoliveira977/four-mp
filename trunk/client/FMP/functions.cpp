#include <math.h>
#include "Hook\types.h"
#include "Hook\classes.h"
#include "Hook\scripting.h"
#include "log.h"
#include "VehicleManager.h"

extern FVehicle gVehicle[MAX_VEHICLES];

using namespace Natives;

/*int FMPHook::GetCarDrive(Vehicle car)
{
	Log::Debug("CraDrive %d", car);
	if(!DoesVehicleExist(car)) { Log::Debug("DOES"); return 1; }
	Log::Debug("CarDrive x%dx", 1);
	if(IsCarStopped(car)) { Log::Debug("STOP"); return 0; }
	Vector3 v;
	Log::Debug("CarDrive x%dx", 2);
	GetCarSpeedVector(car, &v, 1);
	Log::Debug("CarDrive x%dx", 3);
	float x = floor(v.X * 1000 + 0.5)/1000;
	float y = floor(v.Y * 1000 + 0.5)/1000;
	Log::Debug("CarDrive x%dx - %fx%f", x, y);
	if(abs(x) > abs(y))
	{
		// ��� �������� � (��������)
		if(x < 0)
			return -1;
	}
	else
	{
		// ��� �������� Y
		if(y < 0)
			return -1;
	}
	Log::Debug("CarDrive x%dx", 5);
	return 1;
}*/

void GetTarget(float ax, float az, float *x, float *y, float *z, float mn = 5)
{
	float a, b, c, s;

	c = sqrt(2.8f * 2.0f)*mn;
	
	s = c * cos(ax);
	b = s * cos(az);
	a = sqrt( (b*b) + (s*s) - ( 2*b*s * cos(az) ) );
	c = sqrt( ((s*s)/(cos(ax)*cos(ax))) - (s*s) );

	if(ax < 0 || ax >= 180) c = -c;
	if(((az >= 90 && az <= 270) || az <= -90) && b > 0) b=-b;
	if(((az >= 0 && az <= 180) || az <= - 180) && az > 0) a=-a;

	*x=a; *y=b; *z=c;
}

/*void FMPHook::GetCamTargetedCoords(float *x, float *y, float *z)
{
	Log::Info("TARGET %d", 0);
	Camera cam;
	float cx, cy, cz;
	float ax, ay, az;
	float a, b, c;
	Log::Info("TARGET %d", 1);
	GetGameCam(&cam);
	Log::Info("TARGET %d", 11);
	GetCamPos(cam, &cx, &cy, &cz);
	Log::Info("TARGET %d", 12);
	GetCamRot(cam, &ax, &ay, &az);
	Log::Info("TARGET %d", 2);
	GetTarget(ax, az, &a, &b, &c);
	Log::Info("TARGET %d", 3);
	*x = a + cx;
	*y = b + cy;
	*z = c + cz;
	Log::Info("TARGET %d", 4);
}*/

float GetDist(float x1,float y1,float z1,float x2, float y2, float z2)
{
	return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
}

short _GetClosestCar(float x, float y, float z, float r)
{
	float min = r;
	short id = INVALID_VEHICLE_INDEX;
	for (short i = 0; i < MAX_VEHICLES; i++)
	{
		if(gVehicle[i].exist == 1)
		{
			float d = GetDist(x, y, z, gVehicle[i].position[0], gVehicle[i].position[1], gVehicle[i].position[2]);
			if(d < min)
			{
				min = d;
				id = i;
			}
		}
	}
	return id;
}

short GetPlayerCar(Vehicle car)
{
	for(int i = 0; i < MAX_VEHICLES; i++)
	{
		if(gVehicle[i].exist == 1)
		{
			if(car == gVehicle[i].CarID)
				return i;
		}
	}
	return -1;
}