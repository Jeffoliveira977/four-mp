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
			wcscpy(msg, L"   W   N   e   Z   R   o   S   :    Hello");
			msg[0] = msg[4] = msg[8] = msg[12] = msg[16] = msg[20] = msg[24] = msg[28] = msg[32] = 1;
			*(DWORD*)(msg+1) = 0xFFFF0000;
			*(DWORD*)(msg + 5) = 0xFF00FF00;
			*(DWORD*)(msg + 9) = 0xFF0000FF;
			*(DWORD*)(msg + 13) = 0xFFFFFF00;
			*(DWORD*)(msg + 17) = 0xFF00FFFF;
			*(DWORD*)(msg + 21) = 0xFFFF00FF;
			*(DWORD*)(msg + 25) = 0xFFFFFFFF;
			*(DWORD*)(msg + 29) = 0xFF000000;
			*(DWORD*)(msg + 33) = 0xFFFFFFFF;
			pChat->AddMessage(msg);
		}
		else if(GetAsyncKeyState(113)!=0)
		{
			Log::Info("F2");
			wchar_t msg[64];
			wcscpy(msg, L"   DEM@N777:    Hello");
			msg[0] = 1;
			*(DWORD*)(msg+1) = 0xFF0000FF;
			msg[13] = 1;
			*(DWORD*)(msg + 14) = 0xFFFFDD00;
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
		else if(GetAsyncKeyState(33) != 0)
		{
			pChat->ScrollUp();
		}
		else if(GetAsyncKeyState(34) != 0)
		{
			pChat->ScrollDown();
		}
		else if(GetAsyncKeyState(35) != 0)
		{
			pChat->ScrollEnd();
		}
		Wait(50);
	}
}
