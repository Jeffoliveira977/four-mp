#include "fmpconcommands.h"
#include "ConsoleCore.h"
#include "ConsoleScreen.h"
#include "ScriptCommandHandler.h"
#include "..\VirtualMachineManager.h"

extern ConsoleCore concore;
extern ConsoleScreen conscreen;
extern ScriptCommandHandler cmdhandler;
extern VirtualMachineManager vmm;

void ConCmdFsList(unsigned char numargs)
{
	conscreen.Print("Loaded filterscripts:\n---------------------");
	char *string;
	for (unsigned char i = 0; i <= MAX_FILTERSCRIPTS; i++)
	{
		if (vmm.GetFilterScriptInfoString(i, string))
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