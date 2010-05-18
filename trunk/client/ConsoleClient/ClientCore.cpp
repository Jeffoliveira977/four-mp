#define _CRT_RAND_S
#include <stdlib.h>

#include "../Shared/ClientCore.h"
#include "logging.h"
#include "../../Shared/Console/ConsoleCore.h"
#include "../../Shared/Console/ConsoleScreen.h"
#include "con_fmpcommands.h"
#include "con_fmpcvarhooks.h"
#include "../Shared/NetworkManager.h"

extern ConsoleCore concore;
extern ConsoleScreen conscreen;
extern NetworkManager nm;

ClientCore::ClientCore(void)
{
	strcpy(name, "unnamed");
	index = -1; // Should be defined in PlayerManager.h
	isrunning = false;
	sessionkey = 0;
}

ClientCore::~ClientCore(void)
{
}

bool ClientCore::Load(void)
{
	concore.SetOutputFunction(PrintToConsole);
	concore.SetExecPath("cfg/");
	conscreen.SetCaption("FOUR-MP");
	concore.RegisterStandardLibrary();
	concore.AddConCmd("connect", ConCmdConnect, "Connect to specified server.", 0);
	concore.AddConCmd("exit", ConCmdQuit, "Exit the engine.", 0);
	ConVar *namecvar = concore.AddConVar("name", "unnamed", "Current user name.", 0);
	namecvar->HookChange(ConVarHookName);
	concore.AddConCmd("quit", ConCmdQuit, "Exit the engine.", 0);
	rand_s(&sessionkey);
	nm.Load();
	concore.InterpretLine("exec autoexec.cfg");
	isrunning = true;
	return true;
}

bool ClientCore::IsRunning(void)
{
	return isrunning;
}

void ClientCore::Tick(void)
{
	conscreen.CheckUserInput();
	nm.Tick();
	Sleep(100);
}

void ClientCore::Unload(void)
{
	nm.Unload();
}

void ClientCore::Shutdown(void)
{
	isrunning = false;
}

char *ClientCore::GetName(void)
{
	if (name == NULL)
	{
		return NULL;
	}
	char *tempstring = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(tempstring, name);
	return tempstring;
}

short ClientCore::GetIndex(void)
{
	return index;
}

bool ClientCore::SetIndex(const short i)
{
	index = i;
	return true;
}

unsigned int ClientCore::GetSessionKey(void)
{
	return sessionkey;
}