#define _CRT_RAND_S
#include <stdlib.h>

#include "ClientCore.h"
#include "../../Shared/Console/ConsoleCore.h"
#include "con_sharedcommands.h"
#include "con_sharedcvarhooks.h"
#include "NetworkManager.h"
#if defined (FMP_CLIENT)
#include "../FMP/log.h"
#elif defined (FMP_CONSOLE_CLIENT)
#include "../ConsoleClient/logging.h"
#include "../../Shared/Console/ConsoleScreen.h"
#endif

extern ConsoleCore concore;
extern NetworkManager nm;
#if defined (FMP_CONSOLE_CLIENT)
extern ConsoleScreen conscreen;
#endif

void PrintToConsole(const char *string, ...);

ClientCore::ClientCore(void)
{
	gamestate = GameStateLoading;
	strcpy(name, "unnamed");
	namecvar = NULL;
	index = -1; // Should be defined in PlayerManager.h
	sessionkey = 0;
#if defined (FMP_CLIENT)
	inputstate = InputStateGame;
#endif
}

ClientCore::~ClientCore(void)
{
	if (namecvar)
	{
		delete namecvar;
	}
}

bool ClientCore::Load(void)
{
	concore.SetOutputFunction(PrintToConsole);
	concore.RegisterStandardLibrary();
	concore.AddConCmd("connect", ConCmdConnect, "Connect to specified server.", 0);
	concore.AddConCmd("exit", ConCmdQuit, "Exit the engine.", 0);
	namecvar = concore.AddConVar("name", "unnamed", "Current user name.", 0);
	namecvar->HookChange(ConVarHookName);
	concore.AddConCmd("quit", ConCmdQuit, "Exit the engine.", 0);
	rand_s(&sessionkey);
	nm.Load();
#if defined (FMP_CLIENT)
	concore.SetExecPath("FMP/cfg/");
	inputstate = InputStateGui;
#elif defined (FMP_CONSOLE_CLIENT)
	concore.SetExecPath("cfg/");
	conscreen.SetCaption("FOUR-MP");
#endif
	gamestate = GameStateOffline;
	concore.InterpretLine("exec autoexec.cfg");
	return true;
}

bool ClientCore::IsRunning(void)
{
	return gamestate != GameStateUnloading;
}

void ClientCore::Tick(void)
{
	nm.Tick();
#if defined (FMP_CONSOLE_CLIENT)
	conscreen.CheckUserInput();
	Sleep(100);
#endif
}

void ClientCore::Unload(void)
{
	nm.Unload();
	delete namecvar;
}

void ClientCore::Shutdown(void)
{
	gamestate = GameStateUnloading;
}

GameState ClientCore::GetGameState(void)
{
	return gamestate;
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

bool ClientCore::SetGameState(const GameState state)
{
	gamestate = state;
	return true;
}

bool ClientCore::SetName(const char string[MAX_PLAYER_NAME_LENGTH])
{
	if (name == NULL)
	{
		return false;
	}
	return namecvar->SetValue(string);
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

#if defined (FMP_CLIENT)
InputState ClientCore::GetInputState(void)
{
	return inputstate;
}

bool ClientCore::SetInputState(const InputState state)
{
	inputstate = state;
	return true;
}
#endif