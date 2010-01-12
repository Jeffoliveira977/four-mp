/*
*	Created:			26.10.09
*	Created by:			009 & WNeZRoS
*	Last Modifed:		28.10.09
*/
#define main_cpp
// --------------------------------------------------
// includes
// --------------------------------------------------
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <math.h>
// our includes

#include "log.h"
#include "Hook\types.h"
#include "Hook\classes.h"
#include "Hook\hook.h"
#include "Hook\scripting.h"
#include "rpc.h"
#include "functions.h"
#include "main.h"
// RakNet
#include "net\RakNetworkFactory.h"
#include "net\RakPeerInterface.h"
#include "net\MessageIdentifiers.h"
#include "net\BitStream.h"
#include "net\NetworkIDObject.h"
#include "net\NetworkIDManager.h"

#include "Check\check.h"
#include "d3d9\d3d9hook.h"
/* ----------------------------------------------------------------------------------------------------- */
/*                                     Потоки для работы с игрой                                         */
/* ----------------------------------------------------------------------------------------------------- */
FMPHook HOOK;
RakPeerInterface *net;
SystemAddress servAddr;

int mp_state = -2;
int MyID = 0;
int LastUpdate = 0;
bool myEnter = 0;
bool cheats = 0;
int sel_cl = 0;

char model_select_text[128];

FPlayer gPlayer[MAX_PLAYERS];
FVehicle gCar[MAX_CARS];
FConfig Conf;
PlayerClass pClass[MAX_PCLASS];
/* ----------------------------------------------------------------------------------------------------- */
/*                                           Ф У Н К Ц И И                                               */
/* ----------------------------------------------------------------------------------------------------- */

using namespace Natives;

void player_dump()
{
	FILE *f = fopen("fmp_player.txt", "a");
	for(int i = 0; i< MAX_PLAYERS; i++)
	{
		if(gPlayer[i].connected == 1)
		{
			fprintf(f, "%s:%d\n%s\n%d\n", gPlayer[i].ip, gPlayer[i].port, gPlayer[i].name, gPlayer[i].model);
			fprintf(f, "%f %f %f %f\n", gPlayer[i].x, gPlayer[i].y, gPlayer[i].z, gPlayer[i].angle); 
			fprintf(f, "%d %d\n", gPlayer[i].last_active, gPlayer[i].sync_state);
			fprintf(f, "%d %d\n", gPlayer[i].car_id, gPlayer[i].seat_id);
			fprintf(f, "---------------------------\n");
		}
	}
	fprintf(f, "|||||||||||||||||||||||||||||||||||||\n-------------------------------\n");
	fclose(f);
}

void car_dump()
{
	FILE *f = fopen("fmp_car.txt", "a");
	for(int i = 0; i< MAX_CARS; i++)
	{
		if(gCar[i].exist == 1)
		{


			fprintf(f, "%d %d %d\n", gCar[i].model, gCar[i].color[0], gCar[i].color[1]);
			fprintf(f, "%f %f %f %f\n", gCar[i].x, gCar[i].y, gCar[i].z, gCar[i].angle); 
			fprintf(f, "---------------------------\n");
		}
	}
	fprintf(f, "|||||||||||||||||||||||||||||||||||||\n-------------------------------\n");
	fclose(f);
}

void trim(char *a)
{
	int nxt = 0;
	int len = strlen(a);
	for(int i=0; i<len; i++)
	{
		Debug("%d = %d '%c'", i, a[i], a[i]);
		if(a[i] > 32)
		{
			a[nxt] = a[i];
			nxt++;
		}
	}
	while(nxt < len)
	{
		a[nxt] = '\0';
		nxt++;
	}
}

void LoadConfig()
{
	Log("LoadConfig");
	FILE *f = fopen("fmp.ini", "r");
	char buff[32];
	fgets(buff, 32, f);
	strcpy(Conf.server, buff);
	trim(Conf.server);
	fgets(buff, 32, f);
	Conf.port = atoi(buff);
	fgets(buff, 32, f);
	strcpy(Conf.Name, buff);
	fclose(f);
	Log("AT '%s:%d' AS '%s'", Conf.server, Conf.port, Conf.Name);

	Conf.SkinSelect = 0;
	Conf.ComponentSelect = 0;
	Conf.NumSkins = 0;
}

void FMPHook::CreateCar(int id, int model, float x, float y, float z, float r, int color1, int color2)
{
	Log("CREATE CAR %d", id);

	RequestModel((eModel)model);
	while(!HasModelLoaded((eModel)model)) wait(0);
	Log("Model LOADED");
	::CreateCar(model, x, y, z, &gCar[id].CarID, 1);
	SetCarHeading(gCar[id].CarID, r);
	gCar[id].exist = 1;
	gCar[id].model = model;
	gCar[id].x = x;
	gCar[id].y = y;
	gCar[id].z = z;
	gCar[id].angle = r;
	gCar[id].color[0] = color1;
	gCar[id].color[1] = color2;
}

Ped FMPHook::_GetPlayerPed()
{
	Ped Player;
	GetPlayerChar(ConvertIntToPlayerIndex(GetPlayerId()), &Player);
	return Player;
}

Vehicle FMPHook::_GetPedVehicle(Ped p)
{
	Vehicle car;
	GetCarCharIsUsing(p, &car);
	return car;
}

void FMPHook::RunMP()
{
	LastUpdate = GetTickCount();
	Log("RunMP");
	AllowEmergencyServices(0);Log("RUN %d", 1);
	SetPedDensityMultiplier(0);Log("RUN %d", 2);
	SetCarDensityMultiplier(0);Log("RUN %d", 3);
	DisableCarGenerators(1);Log("RUN %d", 4);
	DisableCarGeneratorsWithHeli(1);Log("RUN %d", 5);
	SetNoResprays(1);Log("RUN %d", 6);
	SwitchAmbientPlanes(0);
	SwitchArrowAboveBlippedPickups(0);
	SwitchRandomBoats(0);
	SwitchRandomTrains(0);
	SwitchGarbageTrucks(0);

	//SetSleepModeActive(1);
	Log("RunMP: SetStat");
	
	SetIntStat(STAT_ISLANDS_UNLOCKED, 100);
	SetIntStat(STAT_ACTIVITIES_WITH_BRUCIE, 0);
	SetIntStat(STAT_ACTIVITIES_WITH_ROMAN, 0);
	SetIntStat(STAT_MISSIONS_ATTEMPTED, 0);
	SetIntStat(STAT_MISSIONS_FAILED, 0);
	SetIntStat(STAT_MISSIONS_PASSED, 0);

	SetIntStat(STAT_STUNT_JUMPS_COMPLETED, 50);
	SetIntStat(STAT_ACTIVITIES_WITH_JACOB, 0);
	SetIntStat(STAT_ACTIVITIES_WITH_DWAYNE, 0);
	SetIntStat(STAT_ACTIVITIES_WITH_PACKIE, 0);
	SetIntStat(STAT_PIGEONS_EXTERMINATED, 200);
	SetIntStat(STAT_PROGRESS_WITH_DENISE, 0);
	SetIntStat(STAT_PROGRESS_WITH_MICHELLE, 0);
	SetIntStat(STAT_PROGRESS_WITH_HELENA, 0);
	SetIntStat(STAT_PROGRESS_WITH_BARBARA, 0);
	SetIntStat(STAT_PROGRESS_WITH_KATIE, 0);
	SetIntStat(STAT_PROGRESS_WITH_MILLIE, 0);
	Log("RunMP FloatStat");
	SetFloatStat(STAT_TOTAL_PROGRESS, 100);
	SetFloatStat(STAT_ROMAN_LIKE, 100);
	SetFloatStat(STAT_ROMAN_RESPECT, 100);
	SetFloatStat(STAT_ROMAN_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_JACOB_LIKE, 100);
	SetFloatStat(STAT_JACOB_RESPECT, 100);
	SetFloatStat(STAT_JACOB_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_FAUSTIN_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_MANNY_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_ELIZABETA_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_DWAYNE_LIKE, 100);
	SetFloatStat(STAT_DWAYNE_RESPECT, 100);
	SetFloatStat(STAT_DWAYNE_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_BRUCIE_LIKE, 100);
	SetFloatStat(STAT_BRUCIE_RESPECT, 100);
	SetFloatStat(STAT_BRUCIE_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_PLAYBOY_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_ULPC_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_PACKIE_LIKE, 100);
	SetFloatStat(STAT_PACKIE_RESPECT, 100);
	SetFloatStat(STAT_PACKIE_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_RAY_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_GERRY_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_BERNIE_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_BELL_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_GAMBETTI_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_JIMMY_MISSION_PROGRESS, 100);
	SetFloatStat(STAT_CARMEN_ORTIZ_FONDNESS, 100);
	SetFloatStat(STAT_CARMEN_TRUST, 100);
	SetFloatStat(STAT_ALEX_CHILTON_FONDNESS, 100);
	SetFloatStat(STAT_ALEX_TRUST, 100);
	SetFloatStat(STAT_KIKI_JENKINS_FONDNESS, 100);
	SetFloatStat(STAT_KIKI_TRUST, 100);
	SetFloatStat(STAT_MICHELLE_FONDNESS, 100);
	SetFloatStat(STAT_MICHELLE_TRUST, 100);
	SetFloatStat(STAT_KATE_FONDNESS, 100);
	SetFloatStat(STAT_KATE_TRUST, 100);
	SetFloatStat(STAT_GAME_PROGRESS, 100);
	Log("RunMP: Terminate");
	TerminateAllScriptsWithThisName("initial");
	TerminateAllScriptsWithThisName("main");
	TerminateAllScriptsWithThisName("spcellphone");
	TerminateAllScriptsWithThisName("ambairpotarea");
	TerminateAllScriptsWithThisName("ambatmq");
	TerminateAllScriptsWithThisName("ambbar");
	TerminateAllScriptsWithThisName("ambbarrier");
	TerminateAllScriptsWithThisName("ambbeggar");
	TerminateAllScriptsWithThisName("ambblkhawk");
	TerminateAllScriptsWithThisName("ambbouncer");
	TerminateAllScriptsWithThisName("ambbridgepatrol");
	TerminateAllScriptsWithThisName("ambbusker");
	TerminateAllScriptsWithThisName("ambcabaret");
	TerminateAllScriptsWithThisName("ambcargoholddoors");
	TerminateAllScriptsWithThisName("ambchurchdoors");
	TerminateAllScriptsWithThisName("ambclublights");
	TerminateAllScriptsWithThisName("ambcomedyclub");
	TerminateAllScriptsWithThisName("ambcontrolloader");
	TerminateAllScriptsWithThisName("ambcontrolmain");
	TerminateAllScriptsWithThisName("ambdealer");
	TerminateAllScriptsWithThisName("ambfloater");
	TerminateAllScriptsWithThisName("ambgerry3doorlock");
	TerminateAllScriptsWithThisName("ambhelicopter");
	TerminateAllScriptsWithThisName("ambhomelandcordon");
	TerminateAllScriptsWithThisName("ambhomelandfed");
	TerminateAllScriptsWithThisName("ambhomelandsirens");
	TerminateAllScriptsWithThisName("ambhotel");
	TerminateAllScriptsWithThisName("ambinternetcafe");
	TerminateAllScriptsWithThisName("ambjerseydocksgates");
	TerminateAllScriptsWithThisName("ambjimslocks");
	TerminateAllScriptsWithThisName("ambliftdoors");
	TerminateAllScriptsWithThisName("ambnightclubext");
	TerminateAllScriptsWithThisName("ambpimpnpros");
	TerminateAllScriptsWithThisName("ambpoledancer");
	TerminateAllScriptsWithThisName("ambpolrdblk");
	TerminateAllScriptsWithThisName("ambpreacher");
	TerminateAllScriptsWithThisName("ambsavebed");
	TerminateAllScriptsWithThisName("ambshowroom");
	TerminateAllScriptsWithThisName("ambstripclub");
	TerminateAllScriptsWithThisName("ambtaxdpt");
	TerminateAllScriptsWithThisName("ambtaxihail");
	TerminateAllScriptsWithThisName("ambtoiletdoors");
	TerminateAllScriptsWithThisName("ambtunnelcops");
	TerminateAllScriptsWithThisName("ambtv");
	TerminateAllScriptsWithThisName("ambunarea");
	TerminateAllScriptsWithThisName("ambwardrobe");
	TerminateAllScriptsWithThisName("ambwindowlift");
	TerminateAllScriptsWithThisName("computermain");
	TerminateAllScriptsWithThisName("copbootsearch");
	TerminateAllScriptsWithThisName("emergencycall");
	TerminateAllScriptsWithThisName("empiredown");
	TerminateAllScriptsWithThisName("foodserver");
	TerminateAllScriptsWithThisName("garbage_trucks");
	TerminateAllScriptsWithThisName("stunt");

	ClearAreaOfChars(0,0,0, 2000);
	ClearAreaOfCops(0,0,0, 2000);
	Log("RunMP: Ped");

	Ped Player;
	GetPlayerChar(ConvertIntToPlayerIndex(GetPlayerId()), &Player);Log("RUN 1%d", 1);
	RemoveAllCharWeapons(Player);Log("RUN 1%d", 2);
	AddArmourToChar(Player, -1000);Log("RUN 1%d", 3);
	SetCharMoney(Player,0);Log("RUN 1%d", 4);
	SetTimeOfDay(12,0);
	SetMaxWantedLevel(0);
	Log("RunMP End");
}

void FMPHook::GameThread()
{
	Beep(500, 500);
	Debug("GameThread");

	LoadConfig();
	// Мутим RakNet
	Log("MainThread");
	Packet *pack;
	net = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor s(0, 0);
	net->Startup(8, 1, &s, 1);
	// Регим RPC
	Log("MainThread RPC");

	REGISTER_STATIC_RPC(net, ::ConnectPlayer);
	REGISTER_STATIC_RPC(net, ::Disconnect);

	REGISTER_STATIC_RPC(net, ::Check);
	REGISTER_STATIC_RPC(net, ::ErrorConnect);
	REGISTER_STATIC_RPC(net, ::MovePlayer);
	REGISTER_STATIC_RPC(net, ::JumpPlayer);
	REGISTER_STATIC_RPC(net, ::DuckPlayer);

	REGISTER_STATIC_RPC(net, ::CreateVehicle);

	REGISTER_STATIC_RPC(net, ::PlayerCancelEnterInVehicle);
	REGISTER_STATIC_RPC(net, ::PlayerExitFromVehicle);
	REGISTER_STATIC_RPC(net, ::PlayerEnterInVehicle);

	REGISTER_STATIC_RPC(net, ::SwapGun);
	REGISTER_STATIC_RPC(net, ::PlayerFire);
	REGISTER_STATIC_RPC(net, ::PlayerAim);

	REGISTER_STATIC_RPC(net, ::PlayerParams);

	REGISTER_STATIC_RPC(net, ::ClassSync);
	REGISTER_STATIC_RPC(net, ::SyncSkin);
	REGISTER_STATIC_RPC(net, ::SyncSkinVariation);
	REGISTER_STATIC_RPC(net, ::PlayerSpawn);

	mp_state = -1;
	//-------------------
	Log("GameThread");
	
	while(IsThreadAlive())
	{
		//player_dump();
		//car_dump();
		Debug("FIBERFIBERFIBER");
		if(mp_state == -1)
		{
			
		}
		else if(mp_state == 0)
		{
			RunMP();
			mp_state++;
			Log("mp_state++");
			LastUpdate = GetTickCount();

			// Коннект
			Log("MainThread Connect");
			net->Connect(Conf.server, Conf.port, 0, 0, 0);
			servAddr.SetBinaryAddress(Conf.server);
			servAddr.port = Conf.port;
		}
		else if(mp_state == 1 && GetTickCount() - LastUpdate > 15*1000)
		{
			net->Connect(Conf.server, Conf.port, 0, 0, 0);
			LastUpdate = GetTickCount();
		}
		else if(mp_state == 2 && Conf.SkinSelect == 1)
		{
			FreezeCharPosition(_GetPlayerPed(), 1);
			if(GetAsyncKeyState(VK_SHIFT) != 0)
			{
				gPlayer[MyID].model = pClass[sel_cl].model;	
				FreezeCharPosition(_GetPlayerPed(), 0);
				mp_state = 3;

				if(Conf.ComponentSelect == 0)
				{
					RakNet::BitStream bsSend;
					net->RPC("PlayerSpawn",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
					Log("Player Spawn");
				}
			}
			else if(GetAsyncKeyState(VK_RIGHT) != 0)
			{
				sel_cl++;
				if(sel_cl == Conf.NumSkins)
					sel_cl = 0;
				gPlayer[MyID].model = pClass[sel_cl].model;

				RakNet::BitStream bsSend;
				bsSend.Write(gPlayer[MyID].model);
				net->RPC("Select_ModelChanged",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
				Log("Select ModelChanged");
				
				RequestModel((eModel)gPlayer[MyID].model);
				while(!HasModelLoaded((eModel)gPlayer[MyID].model)) wait(1);
				ChangePlayerModel(GetPlayerId(), (eModel)gPlayer[MyID].model);
				SetCharDefaultComponentVariation(_GetPlayerPed());

				sprintf(model_select_text, "Sellected model id %d", sel_cl);
			}
			else if(GetAsyncKeyState(VK_LEFT) != 0)
			{
				sel_cl--;
				if(sel_cl == -1)
					sel_cl = Conf.NumSkins - 1;
				gPlayer[MyID].model = pClass[sel_cl].model;

				RakNet::BitStream bsSend;
				bsSend.Write(gPlayer[MyID].model);
				net->RPC("Select_ModelChanged",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
				Log("Select Model Changed");
				
				RequestModel((eModel)gPlayer[MyID].model);
				while(!HasModelLoaded((eModel)gPlayer[MyID].model)) wait(1);
				ChangePlayerModel(GetPlayerId(), (eModel)gPlayer[MyID].model);
				SetCharDefaultComponentVariation(_GetPlayerPed());

				sprintf(model_select_text, "Sellected model id %d", sel_cl);
			}
		}
		else if(mp_state == 3 && Conf.ComponentSelect == 1)
		{
			FreezeCharPosition(_GetPlayerPed(), 1);
			if(GetAsyncKeyState(VK_SHIFT) != 0)
			{
				FreezeCharPosition(_GetPlayerPed(), 0);
				mp_state = 4;

				for(int i = 0; i < 11; i++)
				{
					gPlayer[MyID].CompD[i] = GetCharDrawableVariation(_GetPlayerPed(), (ePedComponent)i);
					gPlayer[MyID].CompT[i] = GetCharTextureVariation(_GetPlayerPed(), (ePedComponent)i);
				}

				RakNet::BitStream bsSend;
				bsSend.Write(gPlayer[MyID].CompD);
				bsSend.Write(gPlayer[MyID].CompT);
				net->RPC("SyncSkinVariation",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
				Log("SEND SyncSkinVariation");

				RakNet::BitStream bsSend2;
				net->RPC("PlayerSpawn",&bsSend2,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
				Log("Player Spawn");
			}
			else if(GetAsyncKeyState(VK_RIGHT) != 0)
			{
				int t = GetCharTextureVariation(_GetPlayerPed(), (ePedComponent)sel_cl)+1;
				int d = GetCharDrawableVariation(_GetPlayerPed(), (ePedComponent)sel_cl)+1;

				SetCharComponentVariation(_GetPlayerPed(), (ePedComponent)sel_cl, t, d);

				sprintf(model_select_text, "Selected model id %d", sel_cl);
			}
			else if(GetAsyncKeyState(VK_LEFT) != 0)
			{
				int t = GetCharTextureVariation(_GetPlayerPed(), (ePedComponent)sel_cl)-1;
				if(t == -1) t = 0;
				int d = GetCharDrawableVariation(_GetPlayerPed(), (ePedComponent)sel_cl)-1;
				if(d == -1) t = 0;

				SetCharComponentVariation(_GetPlayerPed(), (ePedComponent)sel_cl, t, d);
				sprintf(model_select_text, "Selected model id %d", sel_cl);
			}
			else if(GetAsyncKeyState(VK_DOWN) != 0)
			{
				sel_cl++;
				if(sel_cl == 11)
					sel_cl = 0;
				sprintf(model_select_text, "Selected model id %d", sel_cl);
			}
			else if(GetAsyncKeyState(VK_UP) != 0)
			{
				sel_cl--;
				if(sel_cl == -1)
					sel_cl = 10;
				sprintf(model_select_text, "Selected model id %d", sel_cl);
			}
		}
		else if(mp_state >= 2)
		{
			Debug("MPSTATE");
			if(GetAsyncKeyState(192) != 0)
			{
				//Chat
			
			}
			Debug("CHAT");
			if(GetTickCount() - LastUpdate > 60*1000)
			{
				// Disconnect: Not info from server
				PrintStringWithLiteralStringNow("STRING", "SERVER NOT SEND INFO TO YOU", 5000, 1);
			}
			Debug("LastUpdate");
			MoveSync();
			CarDoSync();
			GunSync();
			StatusSync();
			gPlayer[MyID].last_active = GetTickCount();
		}
		Debug("IFIFIF");
		if(mp_state != 0)
		{
			Debug("PACKIFPACK");
			pack = net->Receive();
			Debug("PACKPACKPACK");
			if(pack)
			{
				Debug("Pack: %s[%d], %s", pack->data, pack->data[0], pack->systemAddress.ToString());
				if(pack->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
				{
					Log("Connect accepted");
					RakNet::BitStream bsSend;
					bsSend.Write(strlen(Conf.Name));
					bsSend.Write(Conf.Name,strlen(Conf.Name));
					net->RPC("ClientConnect",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
					Log("Connect SENDed");
				}
				else if(pack->data[0] == ID_CONNECTION_ATTEMPT_FAILED)
				{
					Log("Can't connect to server");
				}
				else if(pack->data[0] == ID_RPC_REMOTE_ERROR)
				{
					switch (pack->data[1])
					{
					case RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE:
						Log("RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE\n");
						break;
					case RPC_ERROR_OBJECT_DOES_NOT_EXIST:
						Log("RPC_ERROR_OBJECT_DOES_NOT_EXIST\n");
						break;
					case RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE:
						Log("RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE\n");
						break;
					case RPC_ERROR_FUNCTION_NOT_REGISTERED:
						Log("RPC_ERROR_FUNCTION_NOT_REGISTERED\n");
						break;
					case RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED:
						Log("RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED\n");
						break;
					case RPC_ERROR_CALLING_CPP_AS_C:
						Log("RPC_ERROR_CALLING_CPP_AS_C\n");
						break;
					case RPC_ERROR_CALLING_C_AS_CPP:
						Log("RPC_ERROR_CALLING_C_AS_CPP\n");
						break;
					}
				}
			}
			Debug("KILLPACK");
			net->DeallocatePacket(pack);
			Debug("PACKWAITPACK");
		}
		
		wait(500);
		Debug("WAITWAITWAIT");
	}
}

void GameStart()
{
}

void MainThread(void* dummy)
{
	/* Удаляем инфо файлы */
	debug_clear();
	log_clear();

	DWORD d,c;
	/*VirtualProtect((PVOID)(0xDEA1C0+dwLoadOffset),2,PAGE_EXECUTE_READWRITE,&d);
	strcpy((PCHAR)0xDEA1C0+dwLoadOffset,"GTA IV:Four-MultiPlayer");
	VirtualProtect((PVOID)(0xDEA1C0+dwLoadOffset),2,d,&c);

	VirtualProtect((PVOID)(0xDE2D00+dwLoadOffset),2,PAGE_EXECUTE_READWRITE,&d);
	strcpy((PCHAR)0xDE2D00+dwLoadOffset,"platform:/textures/loadingscreensx");
	VirtualProtect((PVOID)(0xDE2D50+dwLoadOffset),2,PAGE_EXECUTE_READWRITE,&d);
	strcpy((PCHAR)0xDE2D50+dwLoadOffset,"platform:/textures/loadingscreensx");
	VirtualProtect((PVOID)(0x175B793+dwLoadOffset),2,PAGE_EXECUTE_READWRITE,&d);*/


	Sleep(1000);
	
	/*while(!*(BYTE*)(0xfc3fff+dwLoadOffset)) { Sleep(100); }
	GameStart();*/

	Debug("START (0x%x)", dwLoadOffset);

	/* Зацепляем наш GtaThread к игре */
	HOOK.AttachGtaThread();

	Log("MainThread End");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) 
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH) 
	{
		if(CheckSum() == 1 && FileExists("fmp.dll") && !FileExists("dsound.dll"))
		{
			DisableThreadLibraryCalls(hModule);
			DetourFunc((BYTE*)(0xCE32AC+dwLoadOffset),(BYTE*)hkDirect3DCreate9, 5);

			_beginthread(&MainThread, 0, NULL );
		}
		else
		{
			MessageBox(NULL, "Вы используете моды, которые могут мешать нормально синхронизировать вас с другими игроками\nПодробное описание ошибки можно прочитать в readme.txt или на сайте four-mp.com",
				"FOUR-MP:Error", MB_OK|MB_ICONSTOP);
			ExitProcess(1);
		}
		
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		Debug("EXIT FMP");
	}
	return TRUE;
}