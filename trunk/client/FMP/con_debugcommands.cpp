#include "con_debugcommands.h"
#include "../Shared/ClientCore.h"
#include "main.h"
#include "Hook\scripting.h"

extern ClientCore client;

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
	gPlayer[client.GetIndex()].health = gPlayer[client.GetIndex()].health + damage;
	Natives::SetCharHealth(gPlayer[client.GetIndex()].PedID, gPlayer[client.GetIndex()].health);
	return;
}