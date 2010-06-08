#define _CRT_RAND_S
#include <stdlib.h>
#include <stdio.h>

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
	wcscpy(name, L"unnamed");
	namecvar = NULL;
	index = -1; // Should be defined in PlayerManager.h
	sessionkey[0] = 0;
	fmpid = -1;
#if defined (FMP_CLIENT)
	inputstate = InputStateGame;
#endif
	GetModuleFileName(NULL, PathToClient, MAX_PATH);
	for (size_t i = wcslen(PathToClient); i >= 0; i--)
	{
		if(PathToClient[i] == L'/' || PathToClient[i] == L'\\')
			break;
		PathToClient[i] = 0;
	}
	swprintf(PathToClient, MAX_PATH, L"%sFMP\\", PathToClient);
	Log::Info(L"Path: %s", PathToClient);
}

ClientCore::~ClientCore(void)
{
}

wchar_t *ClientCore::GetPath()
{
	return PathToClient;
}

void ClientCore::GetPath(const wchar_t *file, wchar_t *path)
{
	swprintf(path, MAX_PATH, L"%s%s", PathToClient, file);
	Log::Info(L"Returned: %s", path);
}

bool ClientCore::Load(void)
{
	concore.SetOutputFunction(PrintToConsole);
	concore.RegisterStandardLibrary();
	concore.AddConCmd(L"connect", ConCmdConnect, L"Connect to specified server.", 0);
	concore.AddConCmd(L"exit", ConCmdQuit, L"Exit the engine.", 0);
	namecvar = concore.AddConVar(L"name", L"unnamed", L"Current user name.", 0);
	namecvar->HookChange(ConVarHookName);
	concore.AddConCmd(L"quit", ConCmdQuit, L"Exit the engine.", 0);
	nm.Load();
#if defined (FMP_CLIENT)
	concore.SetExecPath(L"FMP/cfg/");
	inputstate = InputStateGui;
#elif defined (FMP_CONSOLE_CLIENT)
	concore.SetExecPath(L"cfg/");
	conscreen.SetCaption(L"FOUR-MP");
#endif
	gamestate = GameStateOffline;
	concore.InterpretLine(L"exec autoexec.cfg");
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

int ClientCore::GetFMPID()
{
	return fmpid;
}

void ClientCore::SetFMPID(int fid)
{
	fmpid = fid;
}

wchar_t *ClientCore::GetName(void)
{
	if (name == NULL)
	{
		return NULL;
	}
	wchar_t *tempstring = (wchar_t *)calloc(wcslen(name) + 1, sizeof(wchar_t));
	wcscpy(tempstring, name);
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

bool ClientCore::SetName(const wchar_t string[MAX_PLAYER_NAME_LENGTH])
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

void ClientCore::SetSessionKey(char *key)
{
	strcpy(sessionkey, key);
}

char *ClientCore::GetSessionKey(void)
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