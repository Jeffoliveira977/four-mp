#include "con_debugcommands.h"
#include "../Shared/ClientCore.h"
#include "main.h"
#include "Hook\scripting.h"
#include "Hook\classes.h"

extern ClientCore client;
extern FMPHook HOOK;

void ConCmdAddarmor(ConsoleCore *concore, const unsigned char numargs)
{
	if (client.GetGameState() != GameStateInGame)
	{
		return;
	}
	int change;
	if (numargs == 0)
	{
		concore->Output(L"Usage: addarmor <amount>");
		return;
	}
	if (!concore->GetCmdArg(1, change))
	{
		concore->Output(L"Usage: addarmor <amount>");
		return;
	}
	gPlayer[client.GetIndex()].armor = gPlayer[client.GetIndex()].armor + (char)change;
	Natives::AddArmourToChar(gPlayer[client.GetIndex()].PedID, gPlayer[client.GetIndex()].armor);
	return;
}

void ConCmdGetarmor(ConsoleCore *concore, const unsigned char numargs)
{
	if (client.GetGameState() != GameStateInGame)
	{
		return;
	}
	unsigned int armor;
	Natives::GetCharArmour(gPlayer[client.GetIndex()].PedID, &armor);
	unsigned int maxarmor;
	Natives::GetPlayerMaxArmour(HOOK._GetPlayer(), &maxarmor);
	concore->Output(L"Your armor is %d / %d.", armor, maxarmor);
	return;
}

void ConCmdGethealth(ConsoleCore *concore, const unsigned char numargs)
{
	if (client.GetGameState() != GameStateInGame)
	{
		return;
	}
	unsigned int health;
	Natives::GetCharHealth(gPlayer[client.GetIndex()].PedID, &health);
	concore->Output(L"Your health is %d.", health);
	return;
}

void ConCmdHurtme(ConsoleCore *concore, const unsigned char numargs)
{
	if (client.GetGameState() != GameStateInGame)
	{
		return;
	}
	int damage;
	if (numargs == 0)
	{
		damage = 1;
	}
	else if (!concore->GetCmdArg(1, damage))
	{
		damage = 1;
	}
	damage = 0 - damage;
	gPlayer[client.GetIndex()].health = gPlayer[client.GetIndex()].health + (char)damage;
	Natives::SetCharHealth(gPlayer[client.GetIndex()].PedID, gPlayer[client.GetIndex()].health);
	return;
}