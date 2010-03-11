/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#define DEBUG
#define LOG

#pragma comment (lib ,"squirrel.lib")
#pragma comment (lib ,"sqstdlib.lib")

///////////////////////////////////////////////
//             I N C L U D E S               //
///////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
// RakNet
#include "net\RakNetworkFactory.h"
#include "net\RakPeerInterface.h"
#include "net\MessageIdentifiers.h"
#include "net\BitStream.h"
// Other
#include "main.h"
#include "HandleManager.h"
#include "CoreHandleTypesManager.h"
#include "console\ConsoleCore.h"
#include "console\ConsoleScreen.h"
#include "console\coreconcommands.h"
#include "console\fmpconcommands.h"
#include "PluginManager.h"
#include "VirtualMachineManager.h"
#include "rpc.h"
#include "manager.h"
#include "cfg.h"

///////////////////////////////////////////////
//                V  A  R  S                 //
///////////////////////////////////////////////
HandleManager hm;
CoreHandleTypesManager chtm;
ConsoleCore concore;
ConsoleScreen conscreen;
RakPeerInterface *net;
PluginManager pm;
VirtualMachineManager vmm;
Player gPlayer[MAX_PLAYERS];
Vehicle gVehicle[MAX_VEHICLES];
Server sConf;
PlayerClass pClass[MAX_PCLASS];
int LastCheck = 0;

void SendChatMessage(char *msg, int player, int r, int g, int b)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(gPlayer[i].connected == 1)
		{
			RakNet::BitStream bsSend;
			bsSend.Write(player);
			bsSend.Write(strlen(msg));
			bsSend.Write(msg);
			bsSend.Write(r);
			bsSend.Write(g);
			bsSend.Write(b);
			net->RPC("Chat",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
		}
	}
}	

void print(const char *string, ...)
{
	va_list arglist; 
    va_start(arglist, string);
	char *tempstring = (char *)calloc(_vscprintf(string, arglist) + 1, sizeof(char));
	vsprintf(tempstring, string, arglist); 
	conscreen.Print(tempstring);
	free(tempstring);
	#ifdef LOG
		FILE *f = fopen("server.log", "a");
		SYSTEMTIME time;
		GetSystemTime(&time);
		fprintf(f, "[%02d.%02d %02d:%02d:%02d] ", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond);
		vfprintf(f, string, arglist);
		fprintf(f, "\n");
		fclose(f);
	#endif
    va_end(arglist); 
}

void debug(const char *string, ...)
{
	va_list arglist; 
    va_start(arglist, string); 
	#ifdef DEBUG
		char *tempstring = (char *)calloc(_vscprintf(string, arglist) + 1, sizeof(char));
		vsprintf(tempstring, string, arglist); 
		conscreen.Print(tempstring);
		free(tempstring);
		FILE *f = fopen("server.log", "a");
		SYSTEMTIME time;
		GetSystemTime(&time);
		fprintf(f, "<%02d.%02d %02d:%02d:%02d> ", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond);
		vfprintf(f, string, arglist);
		fprintf(f, "\n");
		fclose(f);
	#endif
    va_end(arglist); 
}

void LoadConfig()
{
	sConf.Run = 1;
	strcpy(sConf.GameMode[0], "blank.nut");
	sConf.Port = 7777;

	CFG *config = new CFG("server.cfg");
	strcpy(sConf.Name, config->GetVara("Name"));
	sConf.Port = config->GetVari("Port");
	strcpy(sConf.GameMode[0], config->GetVara("GameMode0"));
	strcpy(sConf.GameMode[1], config->GetVara("GameMode1"));
	strcpy(sConf.GameMode[2], config->GetVara("GameMode2"));
	strcpy(sConf.GameMode[3], config->GetVara("GameMode3"));
	strcpy(sConf.Lang, config->GetVara("Lang")); 
	strcpy(sConf.Password, config->GetVara("Password")); 
	strcpy(sConf.RconPassword, config->GetVara("RconPassword")); 
	strcpy(sConf.ServerURL, config->GetVara("ServerURL")); 
}

int main()
{
	concore.SetOutputFunction(print);
	conscreen.SetCaption("FOUR-MP");
	//Core console functions
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("cvarlist", ConCmdCvarlist, "Show the list of convars/concommands.", 0));
	hm.AddNewHandle(0, HandleTypeConVar, new ConVar("developer", 0, "Show developer messages.", 0, true, 0, true, 2));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("find", ConCmdFind, "Find concommands with the specified string in their name/help text.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("help", ConCmdHelp, "Find help about a convar/concommand.", 0));
	// FMP console functions
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_list", ConCmdFsList, "Prints details about loaded gamemode/filterscripts.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_load", ConCmdFsLoad, "fs_load <filename> : loads a filterscript", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_load_all", ConCmdFsLoadAll, "Loads all filterscripts", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_pause", ConCmdFsPause, "fs_pause <index> : pauses a loaded filterscript", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_pause_all", ConCmdFsPauseAll, "Pauses all loaded filterscripts", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_reload", ConCmdFsReload, "fs_reload <index> : reloads a filterscript", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_reload_all", ConCmdFsReloadAll, "Reloads all filterscripts", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_unload", ConCmdFsUnload, "fs_unload <index> : unloads a filterscript", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_unload_all", ConCmdFsUnloadAll, "Unloads all filterscripts", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_unpause", ConCmdFsUnpause, "fs_unpause <index> : unpauses a loaded filterscript", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("fs_unpause_all", ConCmdFsUnpauseAll, "Unpauses all disabled filterscripts", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_list", ConCmdPluginList, "Prints details about loaded plugins.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_load", ConCmdPluginLoad, "plugin_load <filename> : loads a plugin", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_load_all", ConCmdPluginLoadAll, "Loads all plugins", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_pause", ConCmdPluginPause, "plugin_pause <index> : pauses a loaded plugin", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_pause_all", ConCmdPluginPauseAll, "Pauses all loaded plugins", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_reload", ConCmdPluginReload, "fs_reload <index> : reloads a plugin", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_reload_all", ConCmdPluginReloadAll, "Reloads all plugins", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_unload", ConCmdPluginUnload, "fs_unload <index> : unloads a plugin", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_unload_all", ConCmdPluginUnloadAll, "Unloads all plugins", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_unpause", ConCmdPluginUnpause, "fs_unpause <index> : unpauses a loaded plugin", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, new ConCmd("plugin_unpause_all", ConCmdPluginUnpauseAll, "Unpauses all disabled plugins", 0));
	print("FOUR-MP. Copyright 2009-2010 Four-mp team.");
	print("Developer build. Loading...");
	LoadConfig();

	// Init RakNet
	net = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor s(sConf.Port, 0);
	net->SetMaximumIncomingConnections(MAX_PLAYERS);
	net->Startup(MAX_PLAYERS, 1, &s, 1);

	// Register RPC
	REGISTER_STATIC_RPC(net, ClientConnect);
	REGISTER_STATIC_RPC(net, Check);
	REGISTER_STATIC_RPC(net, MovePlayer);
	REGISTER_STATIC_RPC(net, DuckPlayer);
	REGISTER_STATIC_RPC(net, JumpPlayer);

	REGISTER_STATIC_RPC(net, PlayerCancelEnterInVehicle);
	REGISTER_STATIC_RPC(net, PlayerExitFromVehicle);
	REGISTER_STATIC_RPC(net, PlayerEnterInVehicle);

	REGISTER_STATIC_RPC(net, PlayerFire);
	REGISTER_STATIC_RPC(net, PlayerAim);
	REGISTER_STATIC_RPC(net, SwapGun);
	REGISTER_STATIC_RPC(net, PlayerParams);

	REGISTER_STATIC_RPC(net, PlayerSpawn);
	REGISTER_STATIC_RPC(net, Select_ModelChanged);
	REGISTER_STATIC_RPC(net, SyncSkinVariation);

	REGISTER_STATIC_RPC(net, Chat);
	
	pm.LoadPlugins();
	vmm.LoadFilterScripts();
	if (!vmm.LoadGameMode(sConf.GameMode[0]))
	{
		print("Can't load gamemode");
		return 1;
	}
	// Body
	Packet *pack;
	debug("Started");
	
	while(sConf.Run == 1)
	{
		conscreen.CheckUserInput();
		pack = net->Receive();
		if(pack)
		{
			debug("Pack: %s[%d], %s", pack->data, pack->data[0], pack->systemAddress.ToString());
			if((pack->data[0] == ID_DISCONNECTION_NOTIFICATION) || (pack->data[0] == ID_CONNECTION_LOST)) // Disconnect
			{
				int index = GetPlayerID(pack->systemAddress);
				print("Player %s[%d] disconnected", gPlayer[GetPlayerID(pack->systemAddress)].name, GetPlayerID(pack->systemAddress));
				Man_PlayerDisconnect(index);
				for(int i=0; i<MAX_PLAYERS;i++)
				{
					if(gPlayer[i].connected == 1)
					{ 
						RakNet::BitStream bsSend;
						bsSend.Write(index);
						net->RPC("Disconnect",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
					}
				}
			}
			else if(pack->data[0] == ID_NEW_INCOMING_CONNECTION)
			{
				debug("New connection from %s:%d", pack->systemAddress.ToString(0), pack->systemAddress.port);
			}
			else if(pack->data[0] == ID_CONNECTION_REQUEST)
			{
				debug("New connection request");
			}
		}
		if(GetTickCount() - LastCheck >= 30000)
		{
			LastCheck = GetTickCount();
			SystemAddress client;

			for(int i = 0; i < MAX_PLAYERS; i++)
			{
				if(gPlayer[i].connected == 1)
				{
					client.SetBinaryAddress(gPlayer[i].ip);
					client.port = gPlayer[i].port;

					RakNet::BitStream bsSend;
					net->RPC("Check",&bsSend,HIGH_PRIORITY,RELIABLE,0,client,false, 0, UNASSIGNED_NETWORK_ID, 0);
					debug("Check %d", i);
				}
			}
		}
		net->DeallocatePacket(pack);
		Sleep(100);
	}
	return 0;
}