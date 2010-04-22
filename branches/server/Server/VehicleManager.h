#pragma once

class VehicleManager
{
public:
	VehicleManager(void);
	~VehicleManager(void);
	unsigned char GetMaxVehicleBufferSize(void);
	unsigned char GetVehicleBufferSize(void);
	bool IsVehicleExists(const unsigned char index);
	bool CreateVehicle(const int model, const float position[3], const float angle, const int color[2], unsigned char &index);
	void SendVehicleInfo(const unsigned char client, const unsigned char index);
	int GetVehicleModel(const unsigned char index);
	bool GetVehiclePosition(const unsigned char index, float (&position)[3]);
	bool GetVehicleAngle(const unsigned char index, float &angle);
	bool GetVehicleColor(const unsigned char index, int (&color)[2]);
	bool SetVehiclePosition(const unsigned char index, const float position[3]);
	bool SetVehicleAngle(const unsigned char index, const float angle);
private:
	struct Vehicle
	{
		int model;
		float position[3];
		float angle;
		int Health, engHealth;
		int DoorState[6];
		int DoorLock[6];
		int color[2];
	};
	unsigned char maxvehiclebuffersize;
	unsigned char vehiclebuffersize;
	Vehicle **vehiclebuffer;
	bool GetVehicleFreeSlot(unsigned char &index);
	bool ResizeVehicleBuffer(Vehicle **&buffer, const unsigned char size);
};
