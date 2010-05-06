#include <stdlib.h>

#include "VehicleManager.h"
#include "../../Shared/Console/common.h"
#include "NetworkManager.h"

extern NetworkManager nm;

VehicleManager::VehicleManager(void)
{
	maxvehiclebuffersize = 64;
	vehiclebuffersize = 0;
}

VehicleManager::~VehicleManager(void)
{
	for (short i = 0; i < vehiclebuffersize; i++)
	{
		if (vehiclebuffer[i] != NULL)
		{
			free(vehiclebuffer[i]);
		}
	}
	free(vehiclebuffer);
}

short VehicleManager::GetMaxVehicleBufferSize(void)
{
	return maxvehiclebuffersize;
}

short VehicleManager::GetVehicleBufferSize(void)
{
	return vehiclebuffersize;
}

bool VehicleManager::IsVehicleExists(const short index)
{
	if ((index < 0) || (index >= vehiclebuffersize))
	{
		return false;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return false;
	}
	return true;
}

short VehicleManager::CreateVehicle(const int model, const float position[3], const float angle, const unsigned char color[2])
{
	short index = this->GetVehicleFreeSlot();
	if (index == INVALID_VEHICLE_INDEX)
	{
		return INVALID_VEHICLE_INDEX;
	}
	if (index >= vehiclebuffersize)
	{
		if (vehiclebuffersize == maxvehiclebuffersize)
		{
			return INVALID_VEHICLE_INDEX;
		}
		if (!ResizeBuffer<Vehicle **, Vehicle *, short>(vehiclebuffer, index + 1))
		{
			return INVALID_VEHICLE_INDEX;
		}
		vehiclebuffer[index] = NULL;
		vehiclebuffersize = index + 1;
	}
	vehiclebuffer[index] = new Vehicle;
	vehiclebuffer[index]->model = model;
	vehiclebuffer[index]->position[0] = position[0];
	vehiclebuffer[index]->position[1] = position[1];
	vehiclebuffer[index]->position[2] = position[2];
	vehiclebuffer[index]->angle = angle;
	vehiclebuffer[index]->color[0] = color[0];
	vehiclebuffer[index]->color[1] = color[1];
	vehiclebuffer[index]->engHealth = 1000;
	vehiclebuffer[index]->Health = 1000;
	vehiclebuffer[index]->DoorLock[0] = 0;
	vehiclebuffer[index]->DoorLock[1] = 0;
	vehiclebuffer[index]->DoorLock[2] = 0;
	vehiclebuffer[index]->DoorLock[3] = 0;
	vehiclebuffer[index]->DoorLock[4] = 0;
	vehiclebuffer[index]->DoorLock[5] = 0;
	if (!nm.SendNewVehicleInfoToAll(index))
	{
		return INVALID_VEHICLE_INDEX;
	}
	return index;
}

int VehicleManager::GetVehicleModel(const short index)
{
	if ((index < 0) || (index >= vehiclebuffersize))
	{
		return INVALID_VEHICLE_MODEL;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return INVALID_VEHICLE_MODEL;
	}
	return vehiclebuffer[index]->model;
}

bool VehicleManager::GetVehiclePosition(const short index, float (&position)[3])
{
	if ((index < 0) || (index >= vehiclebuffersize))
	{
		return false;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return false;
	}
	position[0] = vehiclebuffer[index]->position[0];
	position[1] = vehiclebuffer[index]->position[1];
	position[2] = vehiclebuffer[index]->position[2];
	return true;
}

bool VehicleManager::GetVehicleAngle(const short index, float &angle)
{
	if ((index < 0) || (index >= vehiclebuffersize))
	{
		return false;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return false;
	}
	angle = vehiclebuffer[index]->angle;
	return true;
}

bool VehicleManager::GetVehicleColor(const short index, unsigned char (&color)[2])
{
	if ((index < 0) || (index >= vehiclebuffersize))
	{
		return false;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return false;
	}
	color[0] = vehiclebuffer[index]->color[0];
	color[1] = vehiclebuffer[index]->color[1];
	return true;
}

bool VehicleManager::SetVehiclePosition(const short index, const float position[3])
{
	if (!this->SetVehiclePositionInternal(index, position))
	{
		return false;
	}
	//TODO: sync it.
	return true;
}

bool VehicleManager::SetVehicleAngle(const short index, const float angle)
{
	if (!this->SetVehicleAngleInternal(index, angle))
	{
		return false;
	}
	//TODO: sync it.
	return true;
}

short VehicleManager::GetVehicleFreeSlot(void)
{
	short index;
	for (index = 0; index < vehiclebuffersize; index++)
	{
		if (vehiclebuffer[index] == NULL)
		{
			return index;
		}
	}
	if (vehiclebuffersize == maxvehiclebuffersize)
	{
		return INVALID_VEHICLE_INDEX;
	}
	return index;
}

bool VehicleManager::SetVehiclePositionInternal(const short index, const float position[3])
{
	if ((index < 0) || (index >= vehiclebuffersize))
	{
		return false;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return false;
	}
	memcpy(vehiclebuffer[index]->position, position, sizeof(float) * 3);
	return true;
}

bool VehicleManager::SetVehicleAngleInternal(const short index, const float angle)
{
	if ((index < 0) || (index >= vehiclebuffersize))
	{
		return false;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return false;
	}
	vehiclebuffer[index]->angle = angle;
	return true;
}