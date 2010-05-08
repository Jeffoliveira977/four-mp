#pragma once

#define INVALID_VEHICLE_INDEX -1
#define INVALID_VEHICLE_MODEL 0;

class VehicleManager
{
public:
	VehicleManager(void);
	~VehicleManager(void);
	short GetMaxVehicleBufferSize(void);
	short GetVehicleBufferSize(void);
	bool IsVehicleExists(const short index);
	short CreateVehicle(const int model, const float position[3], const float angle, const unsigned char color[2]);
	int GetVehicleModel(const short index);
	bool GetVehiclePosition(const short index, float (&position)[3]);
	bool GetVehicleAngle(const short index, float &angle);
	bool GetVehicleColor(const short index, unsigned char (&color)[2]);
	bool SetVehiclePosition(const short index, const float position[3]);
	bool SetVehicleAngle(const short index, const float angle);
private:
	struct Vehicle
	{
		int model; //unsigned int?
		float position[3];
		float angle;
		short Health, engHealth;
		int DoorState[6]; //unsigned char? bool?
		int DoorLock[6]; //unsigned char? bool?
		unsigned char color[2];
	};
	short maxvehiclebuffersize;
	short vehiclebuffersize;
	Vehicle **vehiclebuffer;
	short GetVehicleFreeSlot(void);
	bool SetVehiclePositionInternal(const short index, const float position[3]);
	bool SetVehicleAngleInternal(const short index, const float angle);
	friend class NetworkManager;
};
