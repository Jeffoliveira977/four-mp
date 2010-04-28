/// \file
/// \brief Source file that contains implementation of the four-mp console commands.
/// \details See fmpconcommands.h.
/// \author FaTony

#include "fmpconcommands.h"
#include "CoreHandleTypesManager.h"
#include "PluginManager.h"
#include "VirtualMachineManager.h"

extern CoreHandleTypesManager chtm;
extern PluginManager plugm;
extern VirtualMachineManager vmm;

void ConCmdDynamic(ConsoleCore *concore, unsigned char numargs)
{
	char *cmdname;
	if (!concore->GetCmdArg(0, cmdname))
	{
		return;
	}
	unsigned char numcmds;
	int *indexes = chtm.GetDynamicCommandHandles(cmdname, numcmds);
	if (indexes == NULL)
	{
		concore->Output("Unknown command \"%s\"", cmdname);
		free(cmdname);
		return;
	}
	for (unsigned char i = 0; i < numcmds; i++)
	{
		if (!chtm.ExecuteDynamicCommand(indexes[i], numargs))
		{
			concore->Output("Unknown command \"%s\"", cmdname);
		}
	}
	free(cmdname);
}

void ConCmdFsList(ConsoleCore *concore, unsigned char numargs)
{
	concore->Output("Loaded filterscripts:\n---------------------");
	char *string;
	unsigned char max = vmm.GetVirtualMachineBufferSize();
	for (unsigned char i = 0; i < max; i++)
	{
		string = vmm.GetVirtualMachineInfoString(i);
		if (string != NULL)
		{
			concore->Output(string);
		}
	}
	concore->Output("---------------------");
}

void ConCmdFsLoad(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: fs_load <filename>");
		return;
	}
	char *name;
	if (!concore->GetCmdArg(1, name))
	{
		concore->Output("Usage: fs_load <filename>");
		return;
	}
	if (!vmm.LoadFilterScript(name))
	{
		concore->Output("Unable to load filterscript \"%s\"", name);
		return;
	}
	concore->Output("Filterscript \"%s\" has been loaded successfully", name);
	free(name);
}

void ConCmdFsLoadAll(ConsoleCore *concore, unsigned char numargs)
{
	vmm.LoadFilterScripts();
	concore->Output("Loaded all filterscripts");
}

void ConCmdFsPause(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: fs_pause <index>");
		return;
	}
	int index;
	if (!concore->GetCmdArg(1, index))
	{
		concore->Output("Usage: fs_pause <index>");
		return;
	}
	if (!vmm.PauseVirtualMachine(index))
	{

		concore->Output("Unable to pause filterscript \"%d\"", index);
		return;
	}
	concore->Output("Filterscript \"%d\" disabled", index);
}

void ConCmdFsPauseAll(ConsoleCore *concore, unsigned char numargs)
{
	vmm.PauseVirtualMachines();
	concore->Output("Filterscripts disabled");
}

void ConCmdFsReload(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: fs_reload <index>");
		return;
	}
	int index;
	if (!concore->GetCmdArg(1, index))
	{
		concore->Output("Usage: fs_reload <index>");
		return;
	}
	if (index == 0)
	{
		concore->Output("Can't reload gamemode");
		return;
	}
	if (!vmm.ReloadFilterScript(index))
	{

		concore->Output("Unable to reload filterscript \"%d\"", index);
		return;
	}
	concore->Output("Filterscript \"%d\" has been reloaded successfully", index);
}

void ConCmdFsReloadAll(ConsoleCore *concore, unsigned char numargs)
{
	vmm.ReloadFilterScripts();
	concore->Output("Reloaded all filterscripts");
}

void ConCmdFsUnload(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: fs_unload <index>");
		return;
	}
	int index;
	if (!concore->GetCmdArg(1, index))
	{
		concore->Output("Usage: fs_unload <index>");
		return;
	}
	if (index == 0)
	{
		concore->Output("Can't unload gamemode");
		return;
	}
	if (!vmm.UnloadFilterScript(index))
	{

		concore->Output("Unable to unload filterscript \"%d\", not found", index);
		return;
	}
	concore->Output("Filterscript \"%d\" has been unloaded successfully", index);
}

void ConCmdFsUnloadAll(ConsoleCore *concore, unsigned char numargs)
{
	vmm.UnloadFilterScripts();
	concore->Output("Unloaded all filterscripts");
}

void ConCmdFsUnpause(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: fs_unpause <index>");
		return;
	}
	int index;
	if (!concore->GetCmdArg(1, index))
	{
		concore->Output("Usage: fs_unpause <index>");
		return;
	}
	if (!vmm.UnpauseVirtualMachine(index))
	{

		concore->Output("Unable to unpause filterscript \"%d\"", index);
		return;
	}
	concore->Output("Filterscript \"%d\" enabled", index);
}

void ConCmdFsUnpauseAll(ConsoleCore *concore, unsigned char numargs)
{
	vmm.UnpauseVirtualMachines();
	concore->Output("Filterscripts enabled");
}

void ConCmdPluginList(ConsoleCore *concore, unsigned char numargs)
{
	concore->Output("Loaded plugins:\n---------------------");
	char *string;
	unsigned char max = plugm.GetPluginBufferSize();
	for (unsigned char i = 0; i < max; i++)
	{
		string = plugm.GetPluginInfoString(i);
		if (string != NULL)
		{
			concore->Output(string);
		}
	}
	concore->Output("---------------------");
}

void ConCmdPluginLoad(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: plugin_load <filename>");
		return;
	}
	char *name;
	if (!concore->GetCmdArg(1, name))
	{
		concore->Output("Usage: plugin_load <filename>");
		return;
	}
	if (!plugm.LoadPlugin(name))
	{
		concore->Output("Unable to load plugin \"%s\"", name);
		return;
	}
	concore->Output("Plugin \"%s\" has been loaded successfully", name);
	free(name);
}

void ConCmdPluginLoadAll(ConsoleCore *concore, unsigned char numargs)
{
	plugm.LoadPlugins();
	concore->Output("Loaded all plugins");
}

void ConCmdPluginPause(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: plugin_pause <index>");
		return;
	}
	int index;
	if (!concore->GetCmdArg(1, index))
	{
		concore->Output("Usage: plugin_pause <index>");
		return;
	}
	if (!plugm.PausePlugin(index))
	{

		concore->Output("Unable to pause plugin \"%d\"", index);
		return;
	}
	concore->Output("Plugin \"%d\" disabled", index);
}

void ConCmdPluginPauseAll(ConsoleCore *concore, unsigned char numargs)
{
	plugm.PausePlugins();
	concore->Output("Plugins disabled");
}

void ConCmdPluginReload(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: plugin_reload <index>");
		return;
	}
	int index;
	if (!concore->GetCmdArg(1, index))
	{
		concore->Output("Usage: plugin_reload <index>");
		return;
	}
	if (!plugm.ReloadPlugin(index))
	{

		concore->Output("Unable to reload plugin \"%d\"", index);
		return;
	}
	concore->Output("Plugin \"%d\" has been reloaded successfully", index);
}

void ConCmdPluginReloadAll(ConsoleCore *concore, unsigned char numargs)
{
	plugm.ReloadPlugins();
	concore->Output("Reloaded all plugins");
}

void ConCmdPluginUnload(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: plugin_unload <index>");
		return;
	}
	int index;
	if (!concore->GetCmdArg(1, index))
	{
		concore->Output("Usage: plugin_unload <index>");
		return;
	}
	if (!plugm.UnloadPlugin(index))
	{

		concore->Output("Unable to unload plugin \"%d\", not found", index);
		return;
	}
	concore->Output("Plugin \"%d\" has been unloaded successfully", index);
}

void ConCmdPluginUnloadAll(ConsoleCore *concore, unsigned char numargs)
{
	plugm.UnloadPlugins();
	concore->Output("Unloaded all plugins");
}

void ConCmdPluginUnpause(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: plugin_unpause <index>");
		return;
	}
	int index;
	if (!concore->GetCmdArg(1, index))
	{
		concore->Output("Usage: plugin_unpause <index>");
		return;
	}
	if (!plugm.UnpausePlugin(index))
	{

		concore->Output("Unable to unpause plugin \"%d\"", index);
		return;
	}
	concore->Output("Plugin \"%d\" enabled", index);
}

void ConCmdPluginUnpauseAll(ConsoleCore *concore, unsigned char numargs)
{
	plugm.UnpausePlugins();
	concore->Output("Plugins enabled");
}