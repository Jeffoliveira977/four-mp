#include "fmpconcommands.h"
#include "ConsoleCore.h"
#include "ConsoleScreen.h"
#include "ScriptCommandHandler.h"
#include "..\PluginManager.h"
#include "..\VirtualMachineManager.h"

extern ConsoleCore concore;
extern ConsoleScreen conscreen;
extern ScriptCommandHandler cmdhandler;
extern PluginManager pm;
extern VirtualMachineManager vmm;

void ConCmdFsList(unsigned char numargs)
{
	conscreen.Print("Loaded filterscripts:\n---------------------");
	char *string;
	unsigned char max = vmm.GetMaxFilterScriptIndex();
	for (unsigned char i = 0; i <= max; i++)
	{
		if (vmm.GetVirtualMachineInfoString(i, string))
		{
			conscreen.Print(string);
		}
	}
	conscreen.Print("---------------------");
}

void ConCmdFsLoad(unsigned char numargs)
{
	if (numargs == 0)
	{
		conscreen.Print("Usage: fs_load <filename>");
		return;
	}
	char *name;
	if (!concore.GetCmdArg(1, name))
	{
		conscreen.Print("Usage: fs_load <filename>");
		return;
	}
	if (!vmm.LoadFilterScript(name))
	{
		conscreen.Print("Unable to load filterscript \"%s\"", name);
		return;
	}
	conscreen.Print("Filterscript \"%s\" has been loaded successfully", name);
	free(name);
}

void ConCmdFsLoadAll(unsigned char numargs)
{
	vmm.LoadFilterScripts();
	conscreen.Print("Loaded all filterscripts");
}

void ConCmdFsPause(unsigned char numargs)
{
	if (numargs == 0)
	{
		conscreen.Print("Usage: fs_pause <index>");
		return;
	}
	int index;
	if (!concore.GetCmdArg(1, index))
	{
		conscreen.Print("Usage: fs_pause <index>");
		return;
	}
	if (!vmm.PauseVirtualMachine(index))
	{

		conscreen.Print("Unable to pause filterscript \"%d\"", index);
		return;
	}
	conscreen.Print("Filterscript \"%d\" disabled", index);
}

void ConCmdFsPauseAll(unsigned char numargs)
{
	vmm.PauseVirtualMachines();
	conscreen.Print("Filterscripts disabled");
}

void ConCmdFsReload(unsigned char numargs)
{
	if (numargs == 0)
	{
		conscreen.Print("Usage: fs_reload <index>");
		return;
	}
	int index;
	if (!concore.GetCmdArg(1, index))
	{
		conscreen.Print("Usage: fs_reload <index>");
		return;
	}
	if (index == 0)
	{
		conscreen.Print("Can't reload gamemode");
		return;
	}
	if (!vmm.ReloadFilterScript(index))
	{

		conscreen.Print("Unable to reload filterscript \"%d\"", index);
		return;
	}
	conscreen.Print("Filterscript \"%d\" has been reloaded successfully", index);
}

void ConCmdFsReloadAll(unsigned char numargs)
{
	vmm.ReloadFilterScripts();
	conscreen.Print("Reloaded all filterscripts");
}

void ConCmdFsUnload(unsigned char numargs)
{
	if (numargs == 0)
	{
		conscreen.Print("Usage: fs_unload <index>");
		return;
	}
	int index;
	if (!concore.GetCmdArg(1, index))
	{
		conscreen.Print("Usage: fs_unload <index>");
		return;
	}
	if (index == 0)
	{
		conscreen.Print("Can't unload gamemode");
		return;
	}
	if (!vmm.UnloadFilterScript(index))
	{

		conscreen.Print("Unable to unload filterscript \"%d\", not found", index);
		return;
	}
	conscreen.Print("Filterscript \"%d\" has been unloaded successfully", index);
}

void ConCmdFsUnloadAll(unsigned char numargs)
{
	vmm.UnloadFilterScripts();
	conscreen.Print("Unloaded all filterscripts");
}

void ConCmdFsUnpause(unsigned char numargs)
{
	if (numargs == 0)
	{
		conscreen.Print("Usage: fs_unpause <index>");
		return;
	}
	int index;
	if (!concore.GetCmdArg(1, index))
	{
		conscreen.Print("Usage: fs_unpause <index>");
		return;
	}
	if (!vmm.UnpauseVirtualMachine(index))
	{

		conscreen.Print("Unable to unpause filterscript \"%d\"", index);
		return;
	}
	conscreen.Print("Filterscript \"%d\" enabled", index);
}

void ConCmdFsUnpauseAll(unsigned char numargs)
{
	vmm.UnpauseVirtualMachines();
	conscreen.Print("Filterscripts enabled");
}

//void ConCmdPluginList(unsigned char numargs)
//{
//	conscreen.Print("Loaded plugins:\n---------------------");
//	char *string;
//	unsigned char max = pm.GetPluginBufferSize();
//	for (unsigned char i = 0; i < max; i++)
//	{
//		if (pm.GetPluginInfoString(i, string))
//		{
//			conscreen.Print(string);
//		}
//	}
//	conscreen.Print("---------------------");
//}

void ConCmdPluginLoad(unsigned char numargs)
{
	if (numargs == 0)
	{
		conscreen.Print("Usage: plugin_load <filename>");
		return;
	}
	char *name;
	if (!concore.GetCmdArg(1, name))
	{
		conscreen.Print("Usage: plugin_load <filename>");
		return;
	}
	if (!pm.LoadPlugin(name))
	{
		conscreen.Print("Unable to load plugin \"%s\"", name);
		return;
	}
	conscreen.Print("Plugin \"%s\" has been loaded successfully", name);
	free(name);
}

//void ConCmdFsLoadAll(unsigned char numargs)
//{
//	vmm.LoadFilterScripts();
//	conscreen.Print("Loaded all filterscripts");
//}
//
//void ConCmdFsPause(unsigned char numargs)
//{
//	if (numargs == 0)
//	{
//		conscreen.Print("Usage: fs_pause <index>");
//		return;
//	}
//	int index;
//	if (!concore.GetCmdArg(1, index))
//	{
//		conscreen.Print("Usage: fs_pause <index>");
//		return;
//	}
//	if (!vmm.PauseVirtualMachine(index))
//	{
//
//		conscreen.Print("Unable to pause filterscript \"%d\"", index);
//		return;
//	}
//	conscreen.Print("Filterscript \"%d\" disabled", index);
//}
//
//void ConCmdFsPauseAll(unsigned char numargs)
//{
//	vmm.PauseVirtualMachines();
//	conscreen.Print("Filterscripts disabled");
//}
//
//void ConCmdFsReload(unsigned char numargs)
//{
//	if (numargs == 0)
//	{
//		conscreen.Print("Usage: fs_reload <index>");
//		return;
//	}
//	int index;
//	if (!concore.GetCmdArg(1, index))
//	{
//		conscreen.Print("Usage: fs_reload <index>");
//		return;
//	}
//	if (index == 0)
//	{
//		conscreen.Print("Can't reload gamemode");
//		return;
//	}
//	if (!vmm.ReloadFilterScript(index))
//	{
//
//		conscreen.Print("Unable to reload filterscript \"%d\"", index);
//		return;
//	}
//	conscreen.Print("Filterscript \"%d\" has been reloaded successfully", index);
//}
//
//void ConCmdFsReloadAll(unsigned char numargs)
//{
//	vmm.ReloadFilterScripts();
//	conscreen.Print("Reloaded all filterscripts");
//}
//
void ConCmdPluginUnload(unsigned char numargs)
{
	if (numargs == 0)
	{
		conscreen.Print("Usage: plugin_unload <index>");
		return;
	}
	int index;
	if (!concore.GetCmdArg(1, index))
	{
		conscreen.Print("Usage: plugin_unload <index>");
		return;
	}
	if (!pm.UnloadPlugin(index))
	{

		conscreen.Print("Unable to unload plugin \"%d\", not found", index);
		return;
	}
	conscreen.Print("Plugin \"%d\" has been unloaded successfully", index);
}

//void ConCmdFsUnloadAll(unsigned char numargs)
//{
//	vmm.UnloadFilterScripts();
//	conscreen.Print("Unloaded all filterscripts");
//}
//
//void ConCmdFsUnpause(unsigned char numargs)
//{
//	if (numargs == 0)
//	{
//		conscreen.Print("Usage: fs_unpause <index>");
//		return;
//	}
//	int index;
//	if (!concore.GetCmdArg(1, index))
//	{
//		conscreen.Print("Usage: fs_unpause <index>");
//		return;
//	}
//	if (!vmm.UnpauseVirtualMachine(index))
//	{
//
//		conscreen.Print("Unable to unpause filterscript \"%d\"", index);
//		return;
//	}
//	conscreen.Print("Filterscript \"%d\" enabled", index);
//}
//
//void ConCmdFsUnpauseAll(unsigned char numargs)
//{
//	vmm.UnpauseVirtualMachines();
//	conscreen.Print("Filterscripts enabled");
//}
//
void ConCmdSquirrel(unsigned char numargs)
{
	char *cmdname;
	if (!concore.GetCmdArg(0, cmdname))
	{
		return;
	}
	if (!cmdhandler.IsScriptCommandExist(cmdname))
	{
		conscreen.Print("Unknown command \"%s\"", cmdname);
	}
	if (!cmdhandler.Execute(cmdname, numargs))
	{
		conscreen.Print("Unknown command \"%s\"", cmdname);
	}
	free(cmdname);
}