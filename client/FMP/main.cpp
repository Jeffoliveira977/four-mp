/*
*	Created:			26.10.09
*	Created by:			009 & WNeZRoS
*	Last Modifed:		28.10.09
*/
// --------------------------------------------------
// includes
// --------------------------------------------------
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <math.h>
#include <vector>
// our includes

#include "log.h"
#include "logFile.h"
#include "Hook\types.h"
#include "Hook\classes.h"
#include "Hook\hook.h"
#include "Hook\scripting.h"
#include "functions.h"
#include "main.h"
#include "../Shared/ClientCore.h"
#include "../Shared/NetworkManager.h"
#include "masterserver.h"
#include "Check\check.h"
#include "..\..\Shared\Console\ConsoleCore.h"
#include "d3d9/d3d9hook.h"
#include "d3d9/gui.h"
#include "ConsoleWindow.h"

/* ----------------------------------------------------------------------------------------------------- */
/*                                     Потоки для работы с игрой                                         */
/* ----------------------------------------------------------------------------------------------------- */
ClientCore client;
FMPHook HOOK;
HANDLE MainThreadHandle;

ConsoleCore concore;
NetworkManager nm;
LogFile fileLogger;

FMPGUI Gui;
ConsoleWindow conwindow;

bool myEnter = 0;
bool cheats = 0;

FPlayer gPlayer[MAX_PLAYERS];
FVehicle gCar[MAX_CARS];
CHATMSG mChat[64];

int enterChat = -1;
char enterMsg[256];

void patchCode();
extern DWORD dwGameVersion;


/* ----------------------------------------------------------------------------------------------------- */
/*                                           F U N C T I O N S                                           */
/* ----------------------------------------------------------------------------------------------------- */

void FMPHook::CreateCar(short index, unsigned int model, float position[3], float angle, unsigned char color[2])
{
	Log::Info("CREATE CAR %d", index);
	Natives::RequestModel((eModel)model);
	while(!Natives::HasModelLoaded((eModel)model)) wait(0);
	Log::Info("Model LOADED");
	Natives::CreateCar(model, position[0], position[1], position[2], &gCar[index].CarID, 1);
	Natives::SetCarHeading(gCar[index].CarID, angle);
	gCar[index].exist = 1;
	gCar[index].model = model;
	memcpy(gCar[index].position, position, sizeof(float) * 3);
	gCar[index].angle = angle;
	memcpy(gCar[index].color, color, sizeof(unsigned char) * 2);
}

Ped FMPHook::_GetPlayerPed()
{
	Ped Player;
	Natives::GetPlayerChar(Natives::ConvertIntToPlayerIndex(Natives::GetPlayerId()), &Player);
	return Player;
}

Player FMPHook::_GetPlayer()
{
	unsigned int uint = Natives::GetPlayerId();
	Player pl = Natives::ConvertIntToPlayerIndex(uint);
	return pl;
}

Vehicle FMPHook::_GetPedVehicle(Ped p)
{
	Vehicle car;
	Natives::GetCarCharIsUsing(p, &car);
	return car;
}

void FMPHook::InputFreeze(bool e)
{
	Natives::SetPlayerControl(Natives::ConvertIntToPlayerIndex(Natives::GetPlayerId()), !e);
}

void FMPHook::RunMP()
{
	Log::Info("Starting up multiplayer mode.");
	//AllowEmergencyServices(0);
	Natives::SetPedDensityMultiplier(0);
	Natives::SetCarDensityMultiplier(0);
	Natives::DisableCarGenerators(1);
	//DisableCarGeneratorsWithHeli(1);
	Natives::SetNoResprays(1);
	Natives::SwitchAmbientPlanes(0);
	Natives::SwitchArrowAboveBlippedPickups(0);
	Natives::SwitchRandomBoats(0);
	Natives::SwitchRandomTrains(0);
	Natives::SwitchGarbageTrucks(0);

	//SetSleepModeActive(1);
	
	Natives::SetIntStat(STAT_ISLANDS_UNLOCKED, 100);
	Natives::SetIntStat(STAT_ACTIVITIES_WITH_BRUCIE, 0);
	Natives::SetIntStat(STAT_ACTIVITIES_WITH_ROMAN, 0);
	Natives::SetIntStat(STAT_MISSIONS_ATTEMPTED, 0);
	Natives::SetIntStat(STAT_MISSIONS_FAILED, 0);
	Natives::SetIntStat(STAT_MISSIONS_PASSED, 0);

	Natives::SetIntStat(STAT_STUNT_JUMPS_COMPLETED, 50);
	Natives::SetIntStat(STAT_ACTIVITIES_WITH_JACOB, 0);
	Natives::SetIntStat(STAT_ACTIVITIES_WITH_DWAYNE, 0);
	Natives::SetIntStat(STAT_ACTIVITIES_WITH_PACKIE, 0);
	Natives::SetIntStat(STAT_PIGEONS_EXTERMINATED, 200);
	Natives::SetIntStat(STAT_PROGRESS_WITH_DENISE, 0);
	Natives::SetIntStat(STAT_PROGRESS_WITH_MICHELLE, 0);
	Natives::SetIntStat(STAT_PROGRESS_WITH_HELENA, 0);
	Natives::SetIntStat(STAT_PROGRESS_WITH_BARBARA, 0);
	Natives::SetIntStat(STAT_PROGRESS_WITH_KATIE, 0);
	Natives::SetIntStat(STAT_PROGRESS_WITH_MILLIE, 0);
	
	Natives::SetFloatStat(STAT_TOTAL_PROGRESS, 100);
	Natives::SetFloatStat(STAT_ROMAN_LIKE, 100);
	Natives::SetFloatStat(STAT_ROMAN_RESPECT, 100);
	Natives::SetFloatStat(STAT_ROMAN_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_JACOB_LIKE, 100);
	Natives::SetFloatStat(STAT_JACOB_RESPECT, 100);
	Natives::SetFloatStat(STAT_JACOB_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_FAUSTIN_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_MANNY_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_ELIZABETA_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_DWAYNE_LIKE, 100);
	Natives::SetFloatStat(STAT_DWAYNE_RESPECT, 100);
	Natives::SetFloatStat(STAT_DWAYNE_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_BRUCIE_LIKE, 100);
	Natives::SetFloatStat(STAT_BRUCIE_RESPECT, 100);
	Natives::SetFloatStat(STAT_BRUCIE_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_PLAYBOY_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_ULPC_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_PACKIE_LIKE, 100);
	Natives::SetFloatStat(STAT_PACKIE_RESPECT, 100);
	Natives::SetFloatStat(STAT_PACKIE_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_RAY_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_GERRY_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_BERNIE_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_BELL_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_GAMBETTI_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_JIMMY_MISSION_PROGRESS, 100);
	Natives::SetFloatStat(STAT_CARMEN_ORTIZ_FONDNESS, 100);
	Natives::SetFloatStat(STAT_CARMEN_TRUST, 100);
	Natives::SetFloatStat(STAT_ALEX_CHILTON_FONDNESS, 100);
	Natives::SetFloatStat(STAT_ALEX_TRUST, 100);
	Natives::SetFloatStat(STAT_KIKI_JENKINS_FONDNESS, 100);
	Natives::SetFloatStat(STAT_KIKI_TRUST, 100);
	Natives::SetFloatStat(STAT_MICHELLE_FONDNESS, 100);
	Natives::SetFloatStat(STAT_MICHELLE_TRUST, 100);
	Natives::SetFloatStat(STAT_KATE_FONDNESS, 100);
	Natives::SetFloatStat(STAT_KATE_TRUST, 100);
	Natives::SetFloatStat(STAT_GAME_PROGRESS, 100);
	
	Natives::TerminateAllScriptsWithThisName("initial");
	Natives::TerminateAllScriptsWithThisName("main");
	Natives::TerminateAllScriptsWithThisName("spcellphone");
	Natives::TerminateAllScriptsWithThisName("ambairpotarea");
	Natives::TerminateAllScriptsWithThisName("ambatmq");
	Natives::TerminateAllScriptsWithThisName("ambbar");
	Natives::TerminateAllScriptsWithThisName("ambbarrier");
	Natives::TerminateAllScriptsWithThisName("ambbeggar");
	Natives::TerminateAllScriptsWithThisName("ambblkhawk");
	Natives::TerminateAllScriptsWithThisName("ambbouncer");
	Natives::TerminateAllScriptsWithThisName("ambbridgepatrol");
	Natives::TerminateAllScriptsWithThisName("ambbusker");
	Natives::TerminateAllScriptsWithThisName("ambcabaret");
	Natives::TerminateAllScriptsWithThisName("ambcargoholddoors");
	Natives::TerminateAllScriptsWithThisName("ambchurchdoors");
	Natives::TerminateAllScriptsWithThisName("ambclublights");
	Natives::TerminateAllScriptsWithThisName("ambcomedyclub");
	Natives::TerminateAllScriptsWithThisName("ambcontrolloader");
	Natives::TerminateAllScriptsWithThisName("ambcontrolmain");
	Natives::TerminateAllScriptsWithThisName("ambdealer");
	Natives::TerminateAllScriptsWithThisName("ambfloater");
	Natives::TerminateAllScriptsWithThisName("ambgerry3doorlock");
	Natives::TerminateAllScriptsWithThisName("ambhelicopter");
	Natives::TerminateAllScriptsWithThisName("ambhomelandcordon");
	Natives::TerminateAllScriptsWithThisName("ambhomelandfed");
	Natives::TerminateAllScriptsWithThisName("ambhomelandsirens");
	Natives::TerminateAllScriptsWithThisName("ambhotel");
	Natives::TerminateAllScriptsWithThisName("ambinternetcafe");
	Natives::TerminateAllScriptsWithThisName("ambjerseydocksgates");
	Natives::TerminateAllScriptsWithThisName("ambjimslocks");
	Natives::TerminateAllScriptsWithThisName("ambliftdoors");
	Natives::TerminateAllScriptsWithThisName("ambnightclubext");
	Natives::TerminateAllScriptsWithThisName("ambpimpnpros");
	Natives::TerminateAllScriptsWithThisName("ambpoledancer");
	Natives::TerminateAllScriptsWithThisName("ambpolrdblk");
	Natives::TerminateAllScriptsWithThisName("ambpreacher");
	Natives::TerminateAllScriptsWithThisName("ambsavebed");
	Natives::TerminateAllScriptsWithThisName("ambshowroom");
	Natives::TerminateAllScriptsWithThisName("ambstripclub");
	Natives::TerminateAllScriptsWithThisName("ambtaxdpt");
	Natives::TerminateAllScriptsWithThisName("ambtaxihail");
	Natives::TerminateAllScriptsWithThisName("ambtoiletdoors");
	Natives::TerminateAllScriptsWithThisName("ambtunnelcops");
	Natives::TerminateAllScriptsWithThisName("ambtv");
	Natives::TerminateAllScriptsWithThisName("ambunarea");
	Natives::TerminateAllScriptsWithThisName("ambwardrobe");
	Natives::TerminateAllScriptsWithThisName("ambwindowlift");
	Natives::TerminateAllScriptsWithThisName("computermain");
	Natives::TerminateAllScriptsWithThisName("copbootsearch");
	Natives::TerminateAllScriptsWithThisName("emergencycall");
	Natives::TerminateAllScriptsWithThisName("empiredown");
	Natives::TerminateAllScriptsWithThisName("foodserver");
	Natives::TerminateAllScriptsWithThisName("garbage_trucks");
	Natives::TerminateAllScriptsWithThisName("stunt");
	
	
	Natives::ClearAreaOfChars(0,0,0, 2000);
	//ClearAreaOfCops(0,0,0, 2000);

	Ped Player;
	Natives::GetPlayerChar(Natives::ConvertIntToPlayerIndex(Natives::GetPlayerId()), &Player);
	Natives::RemoveAllCharWeapons(Player);
	Natives::AddArmourToChar(Player, -1000);
	Natives::SetCharMoney(Player, 0);
	Natives::SetTimeOfDay(12,0);
	Natives::SetMaxWantedLevel(0);

	Log::Info("Multiplayer mode started.");
}

void FMPHook::GameThread()
{
	Log::Debug("GameThread");
	client.Load();
	Natives::AddHospitalRestart(0,0,0,0,1000);
	Natives::AddHospitalRestart(0,0,100,0,1001);
	Natives::AddHospitalRestart(0,0,0,90,1002);
	Natives::AddHospitalRestart(0,0,100,90,1003);
	Natives::AddHospitalRestart(0,0,0,180,1004);
	Natives::AddHospitalRestart(0,0,100,180,1005);
	Natives::AddHospitalRestart(0,0,0,270,1006);
	Natives::AddHospitalRestart(0,0,100,270,1007);
	InputFreeze(1);

	RunMP();
	
	while(IsThreadAlive() && client.IsRunning())
	{
		client.Tick();
		GameState state = client.GetGameState();
		switch (state)
		{
		case GameStateOffline:
			{
				break;
			}
		case GameStateConnecting:
			{
				/*if(GetTickCount() - LastUpdate > 15*1000)
				{
					nm.ConnectToServer();
				}*/
				break;
			}
		case GameStateSkinSelect:
			{
				Natives::SetPlayerControl(_GetPlayer(), 0);
				if(GetAsyncKeyState(VK_SHIFT) != 0)
				{
					//gPlayer[client.GetIndex()].model = pClass[selectedplayerclass].model;	
					Natives::SetPlayerControl(_GetPlayer(), 1);
					client.SetGameState(GameStateInGame);
					nm.SendPlayerSpawnRequest();
				}
				else if(GetAsyncKeyState(VK_RIGHT) != 0)
				{
					//selectedplayerclass++;
					//gPlayer[client.GetIndex()].model = pClass[selectedplayerclass].model;
					nm.SendPlayerModelChange();
					
					Natives::RequestModel((eModel)gPlayer[client.GetIndex()].model);
					while(!Natives::HasModelLoaded((eModel)gPlayer[client.GetIndex()].model)) wait(1);
					Natives::ChangePlayerModel(_GetPlayer(), (eModel)gPlayer[client.GetIndex()].model);
					Natives::SetCharDefaultComponentVariation(_GetPlayerPed());
				}
				else if(GetAsyncKeyState(VK_LEFT) != 0)
				{
					//selectedplayerclass--;
					//gPlayer[client.GetIndex()].model = pClass[selectedplayerclass].model;
					nm.SendPlayerModelChange();
					
					Natives::RequestModel((eModel)gPlayer[client.GetIndex()].model);
					while(!Natives::HasModelLoaded((eModel)gPlayer[client.GetIndex()].model)) wait(1);
					Natives::ChangePlayerModel(_GetPlayer(), (eModel)gPlayer[client.GetIndex()].model);
					Natives::SetCharDefaultComponentVariation(_GetPlayerPed());
				}
				break;
			}
		case GameStateComponentSelect:
			{
				Natives::SetPlayerControl(_GetPlayer(), 0);
				if(GetAsyncKeyState(VK_SHIFT) != 0)
				{
					Natives::SetPlayerControl(_GetPlayer(), 1);
					client.SetGameState(GameStateInGame);

					for(int i = 0; i < 11; i++)
					{
						gPlayer[client.GetIndex()].compD[i] = Natives::GetCharDrawableVariation(_GetPlayerPed(), (ePedComponent)i);
						gPlayer[client.GetIndex()].compT[i] = Natives::GetCharTextureVariation(_GetPlayerPed(), (ePedComponent)i);
					}
					nm.SendPlayerComponentsChange();
					nm.SendPlayerSpawnRequest();
				}
				else if(GetAsyncKeyState(VK_RIGHT) != 0)
				{
					//int t = Natives::GetCharTextureVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass)+1;
					//int d = Natives::GetCharDrawableVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass)+1;

					//Natives::SetCharComponentVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass, t, d);
				}
				else if(GetAsyncKeyState(VK_LEFT) != 0)
				{
					//int t = Natives::GetCharTextureVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass)-1;
					//if(t == -1) t = 0;
					//int d = Natives::GetCharDrawableVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass)-1;
					//if(d == -1) t = 0;

					//Natives::SetCharComponentVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass, t, d);
				}
				else if(GetAsyncKeyState(VK_DOWN) != 0)
				{
					//selectedplayerclass++;
					//if(selectedplayerclass == 11)
					//	selectedplayerclass = 0;
				}
				else if(GetAsyncKeyState(VK_UP) != 0)
				{
					//if (selectedplayerclass == 0)
					//{
					//	selectedplayerclass = 10;
					//}
					//else
					//{
					//	selectedplayerclass--;
					//}
				}
				break;
			}
		case GameStateInGame:
			{

				//if(GetTickCount() - LastUpdate > 60*1000)
				//{
				//	// Disconnect: Not info from server
				//	Natives::PrintStringWithLiteralStringNow("STRING", "SERVER NOT SEND INFO TO YOU", 5000, 1);
				//}
				MoveSync();
				CarDoSync();
				GunSync();
				StatusSync();

				gPlayer[client.GetIndex()].last_active = GetTickCount();
				break;
			}
		}
		wait(100);
	}
	client.Unload();
	Log::Debug("Exit GameThread");
}

void FMPHook::GetMyPos()
{
	float x, y, z;
	Natives::GetCharCoordinates(_GetPlayerPed(), &x, &y, &z);
	Log::Info("MY POS (%f; %f; %f)", x, y, z);
}

void GetMyPos(ConsoleCore *concore, const unsigned char numargs)
{
	HOOK.GetMyPos();
}

void MainThread(void* dummy)
{
	Log::Debug("MainThread (0x%x)", dwLoadOffset);
	concore.AddConCmd("getmypos", GetMyPos);
	concore.AddConCmd("getpos", GetMyPos);

	HOOK.AttachGtaThread("FOURMP");
	Log::Debug("Attached");

	CloseHandle(MainThreadHandle);
	MainThreadHandle = NULL;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) 
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH) 
	{
		DisableThreadLibraryCalls(hModule);

		Log::SetState( LOG_INFO|LOG_ERROR|LOG_WARNING|LOG_DEBUG );
		Log::AddLogger( &fileLogger );
		Log::AddLogger( &conwindow );

		concore.RegisterStandardLibrary();

		patchCode();
		GetAddresses(dwGameVersion);
		if(dwGameVersion == 0x1060 || dwGameVersion == 0x1051 || dwGameVersion == 0x1050)
		{
			Log::Info("Skipping main menu");
			JmpHook(CGAME_PROCESS_SLEEP, CGAME_PROCESS_START_GAME);
			SetString(GAME_NAME,"GTA IV: FOUR-MP");

			Log::Debug("Set DX9 Hook");
			DetourFunc((BYTE*)ADDRESS_CREATE_DEVICE,(BYTE*)hkDirect3DCreate9, 5);

			Log::Debug("Set script hook");
			DWORD threadId = 0; 
			MainThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MainThread, 0, 0, (LPDWORD)&threadId);
		}
		else
			Log::Info("This version not supported (%x)", dwGameVersion);
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		client.Shutdown();

		if(MainThreadHandle != NULL)
		{
			TerminateThread(MainThreadHandle, 1);
			CloseHandle(MainThreadHandle);
		}
		Log::Debug("EXIT FMP");
	}
	return TRUE;
}