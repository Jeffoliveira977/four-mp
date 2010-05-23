/*
*	Created:			26.10.09
*	Created by:			009 & VoLT & WNeZRoS
*	Last Modifed:		-
*/
#pragma once
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include "../structs.h"
#include "..\..\..\Shared\Network\NetworkProtocol.h"
/* ----------------------------------------------------------------------------------------------------- */
/*					             Классы и структуры работы с потоками                                    */
/* ----------------------------------------------------------------------------------------------------- */
template<typename T> struct sysArray {
	T**	pData;
	unsigned short	wCount;	
	unsigned short	wSize;	
};

enum ThreadStates
{
	ThreadStateIdle,
	ThreadStateRunning,
	ThreadStateKilled,
	ThreadState3,
	ThreadState4
};

struct scrThreadContext
{
	DWORD nThreadId;
	DWORD dwScriptHash;
	ThreadStates eThreadState;
	DWORD dwOpcodeOff;
	DWORD field_10;
	DWORD nStackOff;
	DWORD nTimerA;
	DWORD nTimerB;
	DWORD nTimerC;
	float fWaitTime;
	DWORD field_28;
	DWORD field_2C;
	DWORD field_30;
	DWORD field_34;	
	DWORD field_38;
	DWORD field_3C;
	DWORD field_40;
	DWORD field_44;
	DWORD field_48;
	DWORD field_4C;
	DWORD field_50;
};

class scrThread
{
protected:
	scrThreadContext m_context;
	void* m_pScriptStack;
	DWORD field_5C;
	void* m_protectedStorage; // void*	XLiveBuffer; взято с http://www.gtamodding.com/index.php?title=Memory_Addresses_%28GTA4%29 
	DWORD field_64;
	DWORD field_68;
	char* m_pszExitMessage;
public:
	virtual ~scrThread() {}
	virtual ThreadStates Reset(unsigned int hash,int v2,int i3) = 0;
	virtual ThreadStates Run(int i1) = 0;
	virtual ThreadStates Tick(unsigned int msec) = 0;
	virtual void Kill() = 0;
	
	scrThreadContext *RetContext() { return &m_context; }
};

class GtaThread :
	public scrThread
{
protected:
	char m_bProgramName[24];							// From scripthook
	DWORD field_88;	
	DWORD field_8C;
	DWORD m_nSavedDeatharrestStackOff;
	bool field_94;
	bool m_bScriptSafeForNetworkGame;
	bool m_bThisScriptShouldBeSaved;
	bool m_bPlayerControlOnInMissionCleanup;
	bool m_bClearHelpInMissionCleanup;
	bool m_bMinigameScript;
	bool m_bAllowNonMinigameTextMessages;
	bool m_bAllowOneTimeOnlyCommandToRun;
	bool m_bPaused;
	bool m_bCanBePaused;
	bool field_9E;										// ; handle type (_fA0) 0: ped, 1: object, 2: world point
	bool field_9F;
	HANDLE field_A0;									// HANDLE
	bool m_bCanRemoveBlipsCreatedByAnyScript;
	bool field_A5;
	bool field_A6;
	bool field_A7;
	DWORD field_A8;
	DWORD field_AC;
};

class FMPThread: 
	protected GtaThread
{
private:
	GtaThread *m_pOriginalThread;
	int m_nThreadIndex;

protected:
	char ThreadName[24];


	ThreadStates Reset(unsigned int hash,int v2,int i3);
	ThreadStates Run(int i1);
	ThreadStates Tick(unsigned int msec);

public:
	FMPThread();
	~FMPThread();
	void Kill();

	void AttachGtaThread(char*);
};

class FMPHook: 
	public FMPThread
{
private:
	bool m_bKillRequested;
	ptr m_pPrimaryFiber;
	ptr m_pScriptFiber;
	unsigned int m_nWaitTick;

	static void _stdcall FiberStart(void* parameter);

protected:
	ThreadStates Reset(unsigned int hash,int v2,int i3);
	ThreadStates Run(int i1);

	void wait(unsigned int timeMS);
	
	bool IsThreadAlive();
	void TerminateThisScript();

public:
	FMPHook();
	~FMPHook();
	void Kill();
	void GameThread();

	// -- Help func
	Ped _GetPlayerPed();
	Vehicle _GetPedVehicle(Ped p);
	Player _GetPlayer();
	int GetCarDrive(Vehicle car);
	void GetCamTargetedCoords(float *x, float *y, float *z);
	void GetMyPos();
	bool SafeCheckPlayer(short index, bool bReCreateOnFalse = 1);
	void CheckAndCheck();

	// -- Start mp
	void RunMP();

	// -- Car sync
	void CreateCar(short index, unsigned int model, float position[3], float angle, unsigned char color[2]);

	// -- Other players sync
	void PlayerConnect(char *name, short index, unsigned int model, float position[3]);
	bool SafeCreatePlayer(short index);

	void PlayerDisconnect(short index);
	void PlayerMove(short index, float position[3], float speed);
	void Jump(short index);
	void Duck(short index, bool);
	void PlayerFireAim(short index, int, int, float, float, float, bool);
	void PlayerSwapGun(short index, int);

	void CancelEnterInVehicle(short index);
	void ExitFromVehicle(short index);
	void EnterInVehicle(short index, int, int);

	void PlayerSyncSkin(short index, int);
	void PlayerSyncSkinVariation(short index, int*, int*);

	void xPlayerSpawn(NetworkPlayerSpawnData data);

	void InputFreeze(bool);

	// -- My sync
	void CarDoSync();
	void MoveSync();
	void GunSync();
	void StatusSync();
};
