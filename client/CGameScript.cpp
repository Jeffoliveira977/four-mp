#include "CGameScript.h"

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
		Wait(50);
	}
}
