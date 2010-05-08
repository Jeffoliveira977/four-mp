/*
*	Created:			26.10.09
*	Created by:			009 & WNeZRoS
*	Last Modifed:		28.10.09
*/
// --------------------------------------------------
// includes
// --------------------------------------------------
#include "masterserver.h"

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <math.h>
#include <vector>
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
#include "../../Shared/RakNet/RakNetworkFactory.h"
#include "../../Shared/RakNet/RakPeerInterface.h"
#include "../../Shared/RakNet/MessageIdentifiers.h"
#include "../../Shared/RakNet/BitStream.h"
#include "../../Shared/RakNet/NetworkIDObject.h"
#include "../../Shared/RakNet/NetworkIDManager.h"
#include "../../Shared/RakNet/GetTime.h"

#include "..\..\Shared\Network\NetworkProtocol.h"
#include "Check\check.h"
#include "..\..\Shared\Console\ConsoleCore.h"
#include "d3d9/d3d9hook.h"
#include "d3d9/gui.h"
#include "ConsoleWindow.h"

/* ----------------------------------------------------------------------------------------------------- */
/*                                     Потоки для работы с игрой                                         */
/* ----------------------------------------------------------------------------------------------------- */
FMPHook HOOK;
HANDLE MainThreadHandle, NetworkThreadHandle;

ConsoleCore concore;

RakPeerInterface *net;
SystemAddress servAddr;

FMPGUI Gui;
ConsoleWindow conwindow;

ClientState clientstate;
short MyID = -1;
int LastUpdate = 0;
bool myEnter = 0;
bool cheats = 0;
unsigned char selectedplayerclass = 0;

char model_select_text[128];

FPlayer gPlayer[MAX_PLAYERS];
FVehicle gCar[MAX_CARS];
FConfig Conf;
PlayerClass pClass[MAX_PCLASS];
CHATMSG mChat[64];

int enterChat = -1;
char enterMsg[256];

void patchCode();
extern DWORD dwGameVersion;


/* ----------------------------------------------------------------------------------------------------- */
/*                                           F U N C T I O N S                                           */
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
			fprintf(f, "%f %f %f %f\n", gPlayer[i].position[0], gPlayer[i].position[1], gPlayer[i].position[2], gPlayer[i].angle); 
			fprintf(f, "%d %d\n", gPlayer[i].last_active, gPlayer[i].sync_state);
			fprintf(f, "%d %d\n", gPlayer[i].vehicleindex, gPlayer[i].seat_id);
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
			fprintf(f, "%f %f %f %f\n", gCar[i].position[0], gCar[i].position[1], gCar[i].position[2], gCar[i].angle); 
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
	FILE *f = fopen("FMP\\fmp.ini", "r");
	char buff[32];
	fgets(buff, 32, f);
	strcpy(Conf.server, buff);
	trim(Conf.server);
	fgets(buff, 32, f);
	Conf.port = atoi(buff);
	fgets(buff, 32, f);
	strcpy(Conf.Name, buff);
	fclose(f);

	Conf.SkinSelect = 0;
	Conf.ComponentSelect = 0;
	Conf.NumSkins = 0;
}

void FMPHook::CreateCar(int id, int model, float x, float y, float z, float r, int color1, int color2)
{
	Log("CREATE CAR %d", id);

	/*RequestModel((eModel)model);
	while(!HasModelLoaded((eModel)model)) wait(0);
	Log("Model LOADED");
	::CreateCar(model, x, y, z, &gCar[id].CarID, 1);
	SetCarHeading(gCar[id].CarID, r);*/
	gCar[id].exist = 1;
	gCar[id].model = model;
	gCar[id].position[0] = x;
	gCar[id].position[1] = y;
	gCar[id].position[2] = z;
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

Player FMPHook::_GetPlayer()
{
	unsigned int uint = GetPlayerId();
	Player pl = ConvertIntToPlayerIndex(uint);
	return pl;
}

Vehicle FMPHook::_GetPedVehicle(Ped p)
{
	Vehicle car;
	GetCarCharIsUsing(p, &car);
	return car;
}

void FMPHook::InputFreeze(bool e)
{
	Natives::SetPlayerControl(ConvertIntToPlayerIndex(GetPlayerId()), !e);
}

void FMPHook::RunMP()
{
	LastUpdate = GetTickCount();
	Log("Starting up multiplayer mode.");
	//AllowEmergencyServices(0);
	SetPedDensityMultiplier(0);
	SetCarDensityMultiplier(0);
	DisableCarGenerators(1);
	//DisableCarGeneratorsWithHeli(1);
	SetNoResprays(1);
	SwitchAmbientPlanes(0);
	SwitchArrowAboveBlippedPickups(0);
	SwitchRandomBoats(0);
	SwitchRandomTrains(0);
	SwitchGarbageTrucks(0);

	//SetSleepModeActive(1);
	
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
	//ClearAreaOfCops(0,0,0, 2000);

	Ped Player;
	GetPlayerChar(ConvertIntToPlayerIndex(GetPlayerId()), &Player);
	RemoveAllCharWeapons(Player);
	AddArmourToChar(Player, -1000);
	SetCharMoney(Player, 0);
	SetTimeOfDay(12,0);
	SetMaxWantedLevel(0);

	Log("Multiplayer mode started.");
}

void FMPHook::ConnectToServer(char *ip, unsigned short port)
{
	LastUpdate = GetTickCount();

	// Коннект
	Log("Connecting to server...");
	if(clientstate.game == GameStateOffline)
	{
		if(ip[0] == 0 || port == 0)
		{
			net->Connect(Conf.server, Conf.port, 0, 0, 0);
			servAddr.SetBinaryAddress(Conf.server);
			servAddr.port = Conf.port;
		}
		else
		{
			net->Connect(ip, port, 0, 0, 0);
			servAddr.SetBinaryAddress(ip);
			servAddr.port = port;
		}
	}
	else if(clientstate.game == GameStateConnecting)
	{
		net->Connect(servAddr.ToString(0), servAddr.port, 0, 0, 0);
	}

	clientstate.game = GameStateConnecting;
}

void FMPHook::GameThread()
{
	Debug("GameThread");
	AddHospitalRestart(0,0,0,0,1000);
	AddHospitalRestart(0,0,100,0,1001);
	AddHospitalRestart(0,0,0,90,1002);
	AddHospitalRestart(0,0,100,90,1003);
	AddHospitalRestart(0,0,0,180,1004);
	AddHospitalRestart(0,0,100,180,1005);
	AddHospitalRestart(0,0,0,270,1006);
	AddHospitalRestart(0,0,100,270,1007);

	clientstate.input = InputStateGui;
	InputFreeze(1);

	RunMP();
	
	while(IsThreadAlive() && clientstate.game != GameStateExiting)
	{
		//player_dump();
		//car_dump();

		// Sync
		for(int i = 0; i < MAX_PLAYERS; i++)
		{
			if(!gPlayer[i].connected) continue;
			if(gPlayer[i].PedID == 0 && gPlayer[i].model != 0)
			{
				RequestModel((eModel)gPlayer[i].model);
				while(!HasModelLoaded((eModel)gPlayer[i].model)) wait(1);

				if(gPlayer[i].LocalPlayer)
				{
					Log("Change me");
					ChangePlayerModel(_GetPlayer(), (eModel)gPlayer[i].model);
					
					GetPlayerChar(_GetPlayer(), &gPlayer[i].PedID);
					SetCharDefaultComponentVariation(gPlayer[i].PedID);
					SetCharCoordinates(gPlayer[i].PedID, gPlayer[i].position[0], gPlayer[i].position[1], gPlayer[i].position[2]);
				}
				else
				{
					Log("Create player");
					CreateChar(1, (eModel)gPlayer[i].model, gPlayer[i].position[0], gPlayer[i].position[1], 
						gPlayer[i].position[2], &gPlayer[i].PedID, 1);

					while(DoesCharExist(gPlayer[i].PedID)) wait(1);

					SetCharDefaultComponentVariation(gPlayer[i].PedID);
					GivePedFakeNetworkName(gPlayer[i].PedID, gPlayer[i].name, gPlayer[i].color[1],
						gPlayer[i].color[2], gPlayer[i].color[3], gPlayer[i].color[0]);
				}
			}
		}

		for(int i = 0; i < MAX_CARS; i++)
		{
			if(gCar[i].exist && gCar[i].CarID == 0)
			{
				Log("Create car");
				RequestModel((eModel)gCar[i].model);
				while(!HasModelLoaded((eModel)gCar[i].model)) wait(1);

				::CreateCar(gCar[i].model, gCar[i].position[0], gCar[i].position[1], gCar[i].position[2], &gCar[i].CarID, 1);
				SetCarHeading(gCar[i].CarID, gCar[i].angle);
			}
		}


		switch (clientstate.game)
		{
		case GameStateOffline:
			{
				break;
			}
		case GameStateConnecting:
			{
				if(GetTickCount() - LastUpdate > 15*1000)
				{
					ConnectToServer();
				}
				break;
			}
		case GameStateSkinSelect:
			{
				SetPlayerControl(_GetPlayer(), 0);
				if(GetAsyncKeyState(VK_SHIFT) != 0)
				{
					gPlayer[MyID].model = pClass[selectedplayerclass].model;	
					SetPlayerControl(_GetPlayer(), 1);
					clientstate.game = GameStateInGame;
					if(Conf.ComponentSelect == 1) clientstate.game = GameStateComponentSelect;

					if(Conf.ComponentSelect == 0)
					{
						NetworkPlayerSpawnRequestData data;
						data.playerclassindex = selectedplayerclass;
						RakNet::BitStream bsSend;
						bsSend.Write(data);
						net->RPC("RPC_PlayerSpawn",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
						Log("Player Spawn");
					}
				}
				else if(GetAsyncKeyState(VK_RIGHT) != 0)
				{
					selectedplayerclass++;
					if(selectedplayerclass == Conf.NumSkins)
						selectedplayerclass = 0;
					gPlayer[MyID].model = pClass[selectedplayerclass].model;
					NetworkPlayerModelChangeData data;
					data.model = gPlayer[MyID].model;
					RakNet::BitStream bsSend;
					bsSend.Write(data);
					net->RPC("RPC_Select_ModelChanged",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
					Log("Select ModelChanged");
					
					RequestModel((eModel)gPlayer[MyID].model);
					while(!HasModelLoaded((eModel)gPlayer[MyID].model)) wait(1);
					ChangePlayerModel(_GetPlayer(), (eModel)gPlayer[MyID].model);
					SetCharDefaultComponentVariation(_GetPlayerPed());

					sprintf(model_select_text, "Selected model id %d", selectedplayerclass);
				}
				else if(GetAsyncKeyState(VK_LEFT) != 0)
				{
					if (selectedplayerclass == 0)
					{
						selectedplayerclass = Conf.NumSkins - 1;
					}
					else
					{
						selectedplayerclass--;
					}
					gPlayer[MyID].model = pClass[selectedplayerclass].model;
					NetworkPlayerModelChangeData data;
					data.model = gPlayer[MyID].model;
					RakNet::BitStream bsSend;
					bsSend.Write(data);
					net->RPC("RPC_Select_ModelChanged",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
					Log("Select Model Changed");
					
					RequestModel((eModel)gPlayer[MyID].model);
					while(!HasModelLoaded((eModel)gPlayer[MyID].model)) wait(1);
					ChangePlayerModel(_GetPlayer(), (eModel)gPlayer[MyID].model);
					SetCharDefaultComponentVariation(_GetPlayerPed());

					sprintf(model_select_text, "Selected model id %d", selectedplayerclass);
				}
				break;
			}
		case GameStateComponentSelect:
			{
				SetPlayerControl(_GetPlayer(), 0);
				if(GetAsyncKeyState(VK_SHIFT) != 0)
				{
					SetPlayerControl(_GetPlayer(), 1);
					clientstate.game = GameStateInGame;

					for(int i = 0; i < 11; i++)
					{
						gPlayer[MyID].compD[i] = GetCharDrawableVariation(_GetPlayerPed(), (ePedComponent)i);
						gPlayer[MyID].compT[i] = GetCharTextureVariation(_GetPlayerPed(), (ePedComponent)i);
					}
					NetworkPlayerComponentsChangeData data;
					memcpy(data.compD, gPlayer[MyID].compD, sizeof(int) * 11);
					memcpy(data.compT, gPlayer[MyID].compT, sizeof(int) * 11);
					RakNet::BitStream bsSend;
					bsSend.Write(data);
					net->RPC("RPC_SyncSkinVariation",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
					Log("SEND SyncSkinVariation");

					RakNet::BitStream bsSend2;
					net->RPC("RPC_PlayerSpawn",&bsSend2,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
					Log("Player Spawn");
				}
				else if(GetAsyncKeyState(VK_RIGHT) != 0)
				{
					int t = GetCharTextureVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass)+1;
					int d = GetCharDrawableVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass)+1;

					SetCharComponentVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass, t, d);

					sprintf(model_select_text, "Selected model id %d", selectedplayerclass);
				}
				else if(GetAsyncKeyState(VK_LEFT) != 0)
				{
					int t = GetCharTextureVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass)-1;
					if(t == -1) t = 0;
					int d = GetCharDrawableVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass)-1;
					if(d == -1) t = 0;

					SetCharComponentVariation(_GetPlayerPed(), (ePedComponent)selectedplayerclass, t, d);
					sprintf(model_select_text, "Selected model id %d", selectedplayerclass);
				}
				else if(GetAsyncKeyState(VK_DOWN) != 0)
				{
					selectedplayerclass++;
					if(selectedplayerclass == 11)
						selectedplayerclass = 0;
					sprintf(model_select_text, "Selected model id %d", selectedplayerclass);
				}
				else if(GetAsyncKeyState(VK_UP) != 0)
				{
					if (selectedplayerclass == 0)
					{
						selectedplayerclass = 10;
					}
					else
					{
						selectedplayerclass--;
					}
					sprintf(model_select_text, "Selected model id %d", selectedplayerclass);
				}
				break;
			}
		case GameStateInGame:
			{
				Debug("MPSTATE");
				if(GetTickCount() - LastUpdate > 60*1000)
				{
					// Disconnect: Not info from server
					PrintStringWithLiteralStringNow("STRING", "SERVER NOT SEND INFO TO YOU", 5000, 1);
				}
				MoveSync();
				CarDoSync();
				GunSync();
				StatusSync();

				gPlayer[MyID].last_active = GetTickCount();
				break;
			}
		}
		wait(100);
	}
	Debug("Exit GameThread");
}

void FMPHook::GetMyPos()
{
	float x, y, z;
	GetCharCoordinates(_GetPlayerPed(), &x, &y, &z);
	Log("MY POS (%f; %f; %f)", x, y, z);
}

void GetMyPos(ConsoleCore *concore, const unsigned char numargs)
{
	HOOK.GetMyPos();
}

void MainThread(void* dummy)
{
	Debug("MainThread (0x%x)", dwLoadOffset);
	LoadConfig();
	clientstate.input = InputStateGame;
	concore.AddConCmd("getmypos", GetMyPos);
	concore.AddConCmd("getpos", GetMyPos);

	HOOK.AttachGtaThread("FOURMP");
	Debug("Atached");

	CloseHandle(MainThreadHandle);
	MainThreadHandle = NULL;
}

void NetworkThread(void *dummy)
{
	Packet *pack;
	Debug("Network Thread");
	net = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor s(0, 0);
	net->Startup(8, 1, &s, 1);

	RegisterRPC();

	while(clientstate.game != GameStateExiting)
	{
		pack = net->Receive();
		if(pack)
		{
			Debug("Pack: %s[%d], %s", pack->data, pack->data[0], pack->systemAddress.ToString());
	
			switch(pack->data[0])
			{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					Log("RakNet: Connection accepted. Sending client info...");
					NetworkPlayerConnectionRequestData data;
					strcpy(data.name, Conf.Name);
					RakNet::BitStream bsSend;
					bsSend.Write(data);
					net->RPC("RPC_ClientConnect",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
					Log("Client info has been sent.");
				} break;
			case ID_ALREADY_CONNECTED:
				{
					Log("RakNet: Already connected");
					clientstate.game = GameStateInGame;	
				} break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				{
					Log("RakNet: No free connections");
					clientstate.game = GameStateOffline;
				} break;
			case ID_DISCONNECTION_NOTIFICATION:
				{
					Log("RakNet: Disconnect (Close connection)");
					clientstate.game = GameStateOffline;
				} break;
			case ID_CONNECTION_LOST:
				{
					Log("RakNet: Disconnect (Connection lost)");
					clientstate.game = GameStateOffline;
				} break;
			case ID_CONNECTION_BANNED:
				{
					Log("RakNet: Disconnect (Connection banned)");
					clientstate.game = GameStateOffline;
				} break;
			case ID_INVALID_PASSWORD:
				{
					Log("RakNet: Invalid password");
				} break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				{
					Log("RakNet: Connection failed");
					clientstate.game = GameStateOffline;
				} break;
			case ID_PONG:
				{
					Log("RakNet: Pong");
					RakNetTime time, dataLength;
					RakNet::BitStream pong( pack->data+1, sizeof(RakNetTime), false);
					pong.Read(time);
					dataLength = pack->length - sizeof(unsigned char) - sizeof(RakNetTime);

					MasterServerInfo *tmp_msi = new MasterServerInfo;
					tmp_msi->ping = (unsigned int)(RakNet::GetTime()-time);

					Debug("ID_PONG from SystemAddress:%u:%u.\n", pack->systemAddress.binaryAddress, pack->systemAddress.port);
					Debug("Time is %i\n",time);
					Debug("Ping is %i\n", tmp_msi->ping);
					Debug("Data is %i bytes long.\n", dataLength);
					if (dataLength > 0)
					{
						Debug("Data is %s\n", pack->data+sizeof(unsigned char)+sizeof(RakNetTime));
						unsigned char *data = pack->data+sizeof(unsigned char)+sizeof(RakNetTime);

						sscanf((char*)data, "%[^\1]\1%[^\1]\1%[^\1]\1%d\1%d\1%d\1%[^\1]\1", &tmp_msi->name, &tmp_msi->mode, &tmp_msi->loc, 
							&tmp_msi->players, &tmp_msi->maxplayers, &tmp_msi->password, &tmp_msi->clan);
					}
					Gui.UpdateServer(pack->systemAddress.ToString(1), tmp_msi);
				} break;
			case ID_RPC_REMOTE_ERROR:
				{
					Log("RakNet: RPC remote error");
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
				} break;
			default:
				{
					Log("RakNet: Unknown message (0x%x) [%s]", pack->data[0], pack->data);
				} break;
			}
		}
		net->DeallocatePacket(pack);
	}

	net->Shutdown(300);
	RakNetworkFactory::DestroyRakPeerInterface(net);

	CloseHandle(NetworkThreadHandle);
	NetworkThreadHandle = NULL;
	Debug("Network EXIT");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) 
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH) 
	{
		InitLogs();

		concore.RegisterStandardLibrary();
		patchCode();

		GetAddresses(dwGameVersion);

		if(dwGameVersion == 0x1060 || dwGameVersion == 0x1051 || dwGameVersion == 0x1050)
		{
			Log("Skip load screens");
			JmpHook(CGAME_PROCESS_SLEEP, CGAME_PROCESS_START_GAME);
			SetString(GAME_NAME,"GTA IV: FOUR-MP");

			Debug("Set DX9 Hook");
			DisableThreadLibraryCalls(hModule);
			DetourFunc((BYTE*)ADDRESS_CREATE_DEVICE,(BYTE*)hkDirect3DCreate9, 5);

			Debug("Set script hook");
			DWORD threadId = 0; 
			MainThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MainThread, 0, 0, (LPDWORD)&threadId);
			NetworkThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&NetworkThread, 0, 0, (LPDWORD)&threadId);
		}
		else
			Log("This version not supported (%x)", dwGameVersion);
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		clientstate.game = GameStateExiting;

		if(MainThreadHandle != NULL)
		{
			TerminateThread(MainThreadHandle, 1);
			CloseHandle(MainThreadHandle);
		}
		if(NetworkThreadHandle != NULL)
		{
			TerminateThread(NetworkThreadHandle, 1);
			CloseHandle(NetworkThreadHandle);
		}

		//while(NetworkThreadHandle != NULL) Sleep(10);

		Debug("EXIT FMP");
		CloseLogs();
	}
	return TRUE;
}