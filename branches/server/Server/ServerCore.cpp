#include "ServerCore.h"
#include "logging.h"
#include "HandleManager.h"
#include "CoreHandleTypesManager.h"
#include "../../Shared/Console/ConsoleCore.h"
#include "../../Shared/Console/ConsoleScreen.h"
#include "../../Shared/Console/coreconcommands.h"
#include "fmpconcommands.h"
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
	lastcheck = 0;
}

ServerCore::~ServerCore(void)
{
}

bool ServerCore::Load(void)
{
	concore.SetOutputFunction(PrintToServer);
	concore.SetExecPath("cfg/");
	conscreen.SetCaption("FOUR-MP");
	//Core console functions
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("cvarlist", ConCmdCvarlist, "Show the list of convars/concommands.", 0));
	hm.AddNewHandle(0, HandleTypeConVar, concore.AddConVar("developer", 0, "Show developer messages.", 0, true, 0, true, 2));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("echo", ConCmdEcho, "Echo text to console.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("exec", ConCmdExec, "Execute script file.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("find", ConCmdFind, "Find concommands with the specified string in their name/help text.", 0));
	hm.AddNewHandle(0, HandleTypeConCmd, concore.AddConCmd("help", ConCmdHelp, "Find help about a convar/concommand.", 0));
	// FMP console functions
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
	gamemode = concore.AddConVar("host_gamemode", "", "Current gamemode name.", 0);
	hm.AddNewHandle(0, HandleTypeConVar, gamemode);
	hostname = concore.AddConVar("hostname", "", "Hostname for server.", 0);
	hm.AddNewHandle(0, HandleTypeConVar, hostname);
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
	rconpassword = concore.AddConVar("rcon_password", "", "Remote console password.", 0);
	hm.AddNewHandle(0, HandleTypeConVar, rconpassword);
	componentselect = concore.AddConVar("sv_componentselect", 0, "Enables component select", 0, true, 0, true, 1);
	hm.AddNewHandle(0, HandleTypeConVar, componentselect);
	password = concore.AddConVar("sv_password", "", "Server password for entry into multiplayer games", 0);
	hm.AddNewHandle(0, HandleTypeConVar, password);
	port = concore.AddConVar("sv_port", 7777, "Server port.", 0, true, 0, true, 65535);
	hm.AddNewHandle(0, HandleTypeConVar, port);
	PrintToServer("FOUR-MP. Copyright 2009-2010 Four-mp team.");
	concore.InterpretLine("exec server.cfg");
	//strcpy(sConf.Lang, config->GetVara("Lang"));
	//strcpy(sConf.ServerURL, config->GetVara("ServerURL"));
	int portvalue;
	port->GetValue(portvalue);
	nm.Init(playm.GetMaxPlayers(), portvalue);
	plugm.LoadPlugins();
	vmm.LoadFilterScripts();
	char *gamemodename;
	gamemode->GetValue(gamemodename);
	if (!vmm.LoadGameMode(gamemodename))
	{
		PrintToServer("Can't load gamemode");
		return false;
	}
	gamemodename = vmm.GetGameModeName();
	char *servername;
	hostname->GetValue(servername);
	if (!msm.RegisterServer(portvalue, servername, gamemodename, "World", playm.GetMaxPlayers(), false))
	{
		PrintToServer("Unable to register server");
	}
	running = true;
	debug("Started");
	return true;
}

bool ServerCore::IsRunning(void)
{
	return running;
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
	Sleep(100);
}

bool ServerCore::GetComponentSelectStatus(void)
{
	int status;
	if (!componentselect->GetValue(status))
	{
		return false;
	}
	return (bool)status;
}

void ServerCore::EnableComponentSelect(bool enable)
{
	componentselect->SetValue(enable);
}