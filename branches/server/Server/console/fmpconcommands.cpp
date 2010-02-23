#include "fmpconcommands.h"
#include "ConsoleCore.h"
#include "ConsoleScreen.h"
#include "ScriptCommandHandler.h"

extern ConsoleCore concore;
extern ConsoleScreen conscreen;
extern ScriptCommandHandler cmdhandler;

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