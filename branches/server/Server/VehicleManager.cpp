#include <stdlib.h>

#include "VehicleManager.h"
#include "NetworkManager.h"

extern NetworkManager nm;

VehicleManager::VehicleManager(void)
{
	maxvehiclebuffersize = 64;
	vehiclebuffersize = 0;
}

VehicleManager::~VehicleManager(void)
{
	for (unsigned char i = 0; i < vehiclebuffersize; i++)
	{
		if (vehiclebuffer[i] != NULL)
		{
			free(vehiclebuffer[i]);
		}
	}
	free(vehiclebuffer);
}

unsigned char VehicleManager::GetMaxVehicleBufferSize(void)
{
	return maxvehiclebuffersize;
}

unsigned char VehicleManager::GetVehicleBufferSize(void)
{
	return vehiclebuffersize;
}

bool VehicleManager::IsVehicleExists(const unsigned char index)
{
	if (index >= vehiclebuffersize)
	{
		return false;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return false;
	}
	return true;
}

bool VehicleManager::CreateVehicle(const int model, const float position[3], const float angle, const int color[2], unsigned char &index)
{
	if (!this->GetVehicleFreeSlot(index))
	{
		return false;
	}
	if (index >= vehiclebuffersize)
	{
		if (vehiclebuffersize == maxvehiclebuffersize)
		{
			return false;
		}
		if (!this->ResizeVehicleBuffer(vehiclebuffer, index + 1))
		{
			return false;
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
	nm.SendNewVehicleToAll(index, model, position, angle, color);
	return true;
}

void VehicleManager::SendVehicleInfo(const unsigned char client, const unsigned char index)
{
	if (index >= vehiclebuffersize)
	{
		return;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return;
	}
	nm.SendVehicleInfo(client, index, vehiclebuffer[index]->model, vehiclebuffer[index]->position, vehiclebuffer[index]->angle, vehiclebuffer[index]->color);
}

int VehicleManager::GetVehicleModel(const unsigned char index)
{
	if (index >= vehiclebuffersize)
	{
		return 0;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return 0;
	}
	return vehiclebuffer[index]->model;
}

bool VehicleManager::GetVehiclePosition(const unsigned char index, float (&position)[3])
{
	if (index >= vehiclebuffersize)
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

bool VehicleManager::GetVehicleAngle(const unsigned char index, float &angle)
{
	if (index >= vehiclebuffersize)
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

bool VehicleManager::GetVehicleColor(const unsigned char index, int (&color)[2])
{
	if (index >= vehiclebuffersize)
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

bool VehicleManager::SetVehiclePosition(const unsigned char index, const float position[3])
{
	if (index >= vehiclebuffersize)
	{
		return false;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return false;
	}
	vehiclebuffer[index]->position[0] = position[0];
	vehiclebuffer[index]->position[1] = position[1];
	vehiclebuffer[index]->position[2] = position[2];
	//TODO: sync it?
	return true;
}

bool VehicleManager::SetVehicleAngle(const unsigned char index, const float angle)
{
	if (index >= vehiclebuffersize)
	{
		return false;
	}
	if (vehiclebuffer[index] == NULL)
	{
		return false;
	}
	vehiclebuffer[index]->angle = angle;
	//TODO: sync it?
	return true;
}

bool VehicleManager::GetVehicleFreeSlot(unsigned char &index)
{
	for (index = 0; index < vehiclebuffersize; index++)
	{
		if (vehiclebuffer[index] == NULL)
		{
			return true;
		}
	}
	if (vehiclebuffersize == maxvehiclebuffersize)
	{
		return false;
	}
	return true;
}

bool VehicleManager::ResizeVehicleBuffer(Vehicle **&buffer, const unsigned char size)
{
	Vehicle **tempbuffer = (Vehicle **)realloc(*&buffer, size * sizeof(Vehicle *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}