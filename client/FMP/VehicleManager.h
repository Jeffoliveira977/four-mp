#pragma once

#include "../../Shared/Network/VehicleDefines.h"
#include "Hook/types.h"

class VehicleManager
{
public:
	VehicleManager(void);
	~VehicleManager(void);
};

struct FVehicle
{
	Vehicle CarID;
	bool exist;
	unsigned int model;
	float position[3];
	float angle;
	short health;
	short enginehealth;
	bool doorexists[6];
	bool doorlock[6];
	float doorangle[6];
	unsigned char color[4];
	bool sirenon;
};