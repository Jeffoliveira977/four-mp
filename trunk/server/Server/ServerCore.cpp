#include <time.h>
#include "ServerCore.h"
#include "logging.h"
#include "HandleManager.h"
#include "CoreHandleTypesManager.h"
#include "../../Shared/Console/ConsoleCore.h"
#include "../../Shared/Console/ConsoleScreen.h"
#include "../../Shared/Console/coreconcommands.h"
#include "con_fmpcvarhooks.h"
#include "con_fmpcommands.h"
#include "MasterServerManager.h"
#include "NetworkManager.h"
#include "PluginManager.h"
#include "VirtualMachineManager.h"
#include "PlayerManager.h"

extern HandleManager hm;
extern ConsoleCore concore;
extern ConsoleScreen conscreen;
extern MasterServerManager msm;
extern NetworkManager nm;
extern PluginManager plugm;
extern VirtualMachineManager vmm;
extern PlayerManager playm;

ServerCore::ServerCore(void)
{
	isrunning = false;
	lastcheck = 0;
	hostname = NULL;
	port = 7777;
	lan = 1;
	gamemode = NULL;
	password = NULL;
	rconpassword = NULL;
	componentselect = false;
	componentselectcvar = NULL;
}

ServerCore::~ServerCore(void)
{
	if (hostname)
	{
		free(hostname);
	}
	if (gamemode)
	{
		free(gamemode);
	}
	if (password)
	{
		free(password);
	}
	if (rconpassword)
	{
		free(rconpassword);
	}
}

bool ServerCore::Load(void)
{
	concore.SetOutputFunction(PrintToServer);
	concore.SetExecPath("cfg/");
	conscreen.SetCaption("FOUR-MP");
	//Core console functions
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("alias", ConCmdAlias, "Alias a command.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("cvarlist", ConCmdCvarlist, "Show the list of convars/concommands.", 0));
	hm.AddNewHandle(0, HandleTypeConVar, concore.AddConVar("developer", 0, "Show developer messages.", 0, true, 0, true, 2));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("echo", ConCmdEcho, "Echo text to console.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("exec", ConCmdExec, "Execute script file.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("find", ConCmdFind, "Find concommands with the specified string in their name/help text.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("help", ConCmdHelp, "Find help about a convar/concommand.", 0));
	// FMP console functions
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("exit", ConCmdQuit, "Exit the engine.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_list", ConCmdFsList, "Prints details about loaded gamemode/filterscripts.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_load", ConCmdFsLoad, "fs_load <filename> : loads a filterscript", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_load_all", ConCmdFsLoadAll, "Loads all filterscripts", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_pause", ConCmdFsPause, "fs_pause <index> : pauses a loaded filterscript", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_pause_all", ConCmdFsPauseAll, "Pauses all loaded filterscripts", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_reload", ConCmdFsReload, "fs_reload <index> : reloads a filterscript", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_reload_all", ConCmdFsReloadAll, "Reloads all filterscripts", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_unload", ConCmdFsUnload, "fs_unload <index> : unloads a filterscript", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_unload_all", ConCmdFsUnloadAll, "Unloads all filterscripts", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_unpause", ConCmdFsUnpause, "fs_unpause <index> : unpauses a loaded filterscript", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("fs_unpause_all", ConCmdFsUnpauseAll, "Unpauses all disabled filterscripts", 0));
	ConVar *gamemodecvar = concore.AddConVar("host_gamemode", "", "Current gamemode name.", 0);
	gamemodecvar->HookChange(ConVarHookHostGamemode);
	hm.AddNewHandle(0, HandleTypeConVar, gamemodecvar);
	ConVar *hostnamecvar = concore.AddConVar("hostname", "", "Hostname for server.", 0);
	hostnamecvar->HookChange(ConVarHookHostname);
	hm.AddNewHandle(0, HandleTypeConVar, hostnamecvar);
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_list", ConCmdPluginList, "Prints details about loaded plugins.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_load", ConCmdPluginLoad, "plugin_load <filename> : loads a plugin", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_load_all", ConCmdPluginLoadAll, "Loads all plugins", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_pause", ConCmdPluginPause, "plugin_pause <index> : pauses a loaded plugin", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_pause_all", ConCmdPluginPauseAll, "Pauses all loaded plugins", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_reload", ConCmdPluginReload, "fs_reload <index> : reloads a plugin", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_reload_all", ConCmdPluginReloadAll, "Reloads all plugins", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_unload", ConCmdPluginUnload, "fs_unload <index> : unloads a plugin", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_unload_all", ConCmdPluginUnloadAll, "Unloads all plugins", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_unpause", ConCmdPluginUnpause, "fs_unpause <index> : unpauses a loaded plugin", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("plugin_unpause_all", ConCmdPluginUnpauseAll, "Unpauses all disabled plugins", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("quit", ConCmdQuit, "Exit the engine.", 0));
	ConVar *rconpasswordcvar = concore.AddConVar("rcon_password", "", "Remote console password.", 0);
	rconpasswordcvar->HookChange(ConVarHookRconPassword);
	hm.AddNewHandle(0, HandleTypeConVar, rconpasswordcvar);
	componentselectcvar = concore.AddConVar("sv_componentselect", 0, "Enables component select", 0, true, 0, true, 1);
	componentselectcvar->HookChange(ConVarHookSvComponentselect);
	hm.AddNewHandle(0, HandleTypeConVar, componentselectcvar);
	ConVar *lancvar = concore.AddConVar("sv_lan", 1, "Server is a lan server (no heartbeat, no authentication, no non-class C addresses).", 0, true, 0, true, 1);
	lancvar->HookChange(ConVarHookSvLan);
	hm.AddNewHandle(0, HandleTypeConVar, lancvar);
	ConVar *passwordcvar = concore.AddConVar("sv_password", "", "Server password for entry into multiplayer games", 0);
	passwordcvar->HookChange(ConVarHookSvPassword);
	hm.AddNewHandle(0, HandleTypeConVar, passwordcvar);
	ConVar *portcvar = concore.AddConVar("sv_port", 7777, "Server port.", 0, true, 0, true, 65535);
	portcvar->HookChange(ConVarHookSvPort);
	hm.AddNewHandle(0, HandleTypeConVar, portcvar);
	PrintToServer("FOUR-MP. Copyright 2009-2010 Four-mp team.");
	concore.InterpretLine("exec server.cfg");
	nm.Load(playm.GetMaxPlayers(), port);
	maxplayers = playm.GetMaxPlayers();
	plugm.LoadPlugins();
	vmm.LoadFilterScripts();
	if (!vmm.LoadGameMode(gamemode))
	{
		PrintToServer("Can't load gamemode.");
		return false;
	}
	gamemodename = vmm.GetGameModeName();
	if (!lan)
	{
		msm.Init();
		if (!msm.RegisterServer(port, hostname, gamemodename, "World", maxplayers, password))
		{
			PrintToServer("Unable to register server.");
		}
		lastmasterservercheck = time(0);
	}
	nm.UpdateServerInfo();
	isrunning = true;
	debug("Started");
	return true;
}

bool ServerCore::IsRunning(void)
{
	return isrunning;
}
void ServerCore::Tick(void)
{
	conscreen.CheckUserInput();
	nm.Tick();
	if(GetTickCount() - lastcheck >= 30000)
	{
		lastcheck = GetTickCount();
		nm.CheckClients();
	}
	if (!lan)
	{
		if (time(0) - lastmasterservercheck >= 3600000)
		{
			lastmasterservercheck = time(0);
			if (!msm.RegisterServer(port, hostname, gamemodename, "World", maxplayers, password))
			{
				PrintToServer("Unable to register server.");
			}
		}
	}
	Sleep(100);
}

void ServerCore::Unload(void)
{
	//TODO:
	//1. Politely kick all players.
	//2. Destroy all vehicles.
	vmm.UnloadGameMode(); //3. Unload all virtual machines.
	vmm.UnloadFilterScripts(); //3. Unload all virtual machines.
	plugm.UnloadPlugins(); //4. Unload all plugins.
	nm.Unload();
	//5. Clean up all remaining data - destructors.
}

void ServerCore::Shutdown(void)
{
	isrunning = false;
}

bool ServerCore::IsLAN(void)
{
	return lan;
}

bool ServerCore::IsPasswordProtected(void)
{
	if (password == NULL)
	{
		return false;
	}
	return true;
}

char *ServerCore::GetHostname(void)
{
	if (hostname == NULL)
	{
		return NULL;
	}
	char *tempstring = (char *)calloc(strlen(hostname) + 1, sizeof(char));
	strcpy(tempstring, hostname);
	return tempstring;
}

bool ServerCore::GetComponentSelectStatus(void)
{
	return componentselect;
}

void ServerCore::EnableComponentSelect(bool enable)
{
	componentselect = enable;
	componentselectcvar->SetValue(enable);
}