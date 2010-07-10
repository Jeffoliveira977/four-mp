#include <stdio.h>
#include <windows.h>
#include "./log/log.h"
#include "./log/logFile.h"
#include "./hook/CGameHook.h"
#include "./CGameScript.h"
#include "./CGameD3D.h"
#include "./CChat.h"

LogFile * logfile = NULL;
CGameHook * pGameHook = NULL;
CGameScript * pGameScript = NULL;
CGameD3D * pGameD3D = NULL;
CChat * pChat = NULL;

bool IsFMPNeed()
{
	char *cmdline = GetCommandLineA();
	int cmdlen = strlen(cmdline) - 1;
	for(int i = cmdlen; i != 5; i--)
	{
		if((cmdline[i] == 'p' || cmdline[i] == 'P') && (cmdline[i - 1] == 'm' || cmdline[i - 1] == 'M') && 
			(cmdline[i - 2] == 'f' || cmdline[i - 2] == 'F') && (cmdline[i - 3] == '-' || cmdline[i - 3] == '-') && 
			(cmdline[i - 4] == ' ' || cmdline[i - 4] == ' '))
			{
				return true;
				break;
			}
	}
	return false;
}

void Start(HANDLE hModule)
{
	if(!GetModuleHandleA("xlive.dll"))
	{
		return;
	}

	if(!IsFMPNeed())
	{
		return;
	}
	
	logfile = new LogFile();
	Log::Info("Second attach, run hook");

	pGameScript = new CGameScript;
	pGameD3D = new CGameD3D;
	pChat = new CChat;
	pGameHook = new CGameHook;

	if(!pGameHook->UnprotectData())
	{
		Log::Error("Can't unprotect data");
		exit(0);
	}
	if(!pGameHook->PatchGame())
	{
		Log::Error("Can't patch game");
	}

	if(!pGameHook->InstallXLiveHook())
	{
		Log::Error("Can't hook xlive");
	}

	if(!pGameHook->InstallScriptHook(pGameScript))
	{
		Log::Error("Can't install ScriptHook");
	}
	if(!pGameHook->InstallDirect3DHook(pGameD3D))
	{
		Log::Error("Can't install D3D Hook");
	}
	if(!pGameHook->InstallDirect3DHook(pChat))
	{
		Log::Error("Can't install D3D Hook for chat");
	}
}

void Close()
{
	if(pGameScript) delete pGameScript;
	if(pChat) delete pChat;
	if(pGameD3D) delete pGameD3D;
	if(pGameHook) delete pGameHook;
	if(logfile) delete logfile;
}

