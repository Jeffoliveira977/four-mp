#include "CGameScript.h"
#include "CChat.h"
#include "./log/log.h"

extern CChat *pChat;

CGameScript::CGameScript()
{
	SetName("FOURMP");
}

CGameScript::~CGameScript()
{

}

void CGameScript::GameThread()
{
	while(IsThreadAlive())
	{
		if(GetAsyncKeyState(112)!=0)
		{
			Log::Info("F1");
			wchar_t msg[64];
			wcscpy(msg, L"   WNeZRoS:    Hello");
			msg[0] = 1;
			*(DWORD*)(msg+1) = 0xFFFF0000;
			msg[12] = 1;
			*(DWORD*)(msg + 13) = 0xFF00FF00;
			pChat->AddMessage(msg);
		}
		else if(GetAsyncKeyState(113)!=0)
		{
			Log::Info("F2");
			wchar_t msg[64];
			wcscpy(msg, L"   DEM@N77:    Hello");
			msg[0] = 1;
			*(DWORD*)(msg+1) = 0xFF0000FF;
			msg[12] = 1;
			*(DWORD*)(msg + 13) = 0xFFFFDD00;
			pChat->AddMessage(msg);
		}
		else if(GetAsyncKeyState(114)!=0)
		{
			Log::Info("F3");
			wchar_t msg[64];
			wcscpy(msg, L"   WNeZRoS:    How are you");
			msg[0] = 1;
			*(DWORD*)(msg+1) = 0xFFFF0000;
			msg[12] = 1;
			*(DWORD*)(msg + 13) = 0xFFFFFFFF;
			pChat->AddMessage(msg);
		}
		Wait(50);
	}
}
