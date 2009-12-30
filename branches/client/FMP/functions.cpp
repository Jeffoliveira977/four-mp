#include <math.h>
#include "Hook\types.h"
#include "Hook\classes.h"
#include "Hook\scripting.h"
#include "main.h"
#include "log.h"

using namespace Natives;

int FMPHook::GetCarDrive(Vehicle car)
{
	Debug("CraDrive %d", car);
	if(!DoesVehicleExist(car)) { Debug("DOES"); return 1; }
	Debug("CarDrive x%dx", 1);
	if(IsCarStopped(car)) { Debug("STOP"); return 0; }
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
			return -1;
	}
	else
	{
		// ось движения Y
		if(y < 0)
			return -1;
	}
	Debug("CarDrive x%dx", 5);
	return 1;
}

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

void FMPHook::GetCamTargetedCoords(float *x, float *y, float *z)
{
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
	GetTarget(ax, az, &a, &b, &c);
	Log("TARGET %d", 3);
	*x = a + cx;
	*y = b + cy;
	*z = c + cz;
	Log("TARGET %d", 4);
}

float GetDist(float x1,float y1,float z1,float x2, float y2, float z2)
{
	return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
}

int _GetClosestCar(float x, float y, float z, float r)
{
	float min = r;
	int id = -1;
	for(int i = 0; i < MAX_CARS; i++)
	{
		if(gCar[i].exist == 1)
		{
			float d = GetDist(x, y, z, gCar[i].x, gCar[i].y, gCar[i].z);
			if(d < min)
			{
				min = d;
				id = i;
			}
		}
	}
	return id;
}

int GetPlayerCar(Vehicle car)
{
	for(int i = 0; i < MAX_CARS; i++)
	{
		if(gCar[i].exist == 1)
		{
			if(car == gCar[i].CarID)
				return i;
		}
	}
	return -1;
}