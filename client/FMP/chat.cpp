#include <windows.h>
#include <stdio.h>

#include "log.h"
#include "main.h"
#include "NetworkManager.h"

extern NetworkManager nm;

void AddChatMessage(char *msg, COLOR clr, int playerid = -1)
{
	for(int i = 63; i > 0; i--)
	{
		mChat[i] = mChat[i-1];
	}
	if(playerid >= 0)
		if(gPlayer[playerid].connected == 1)
			sprintf(mChat[0].msg, "%s: %s", gPlayer[playerid].name, msg); 
		else
			sprintf(mChat[0].msg, "$UNK: %s", msg); 
	else
		strcpy(mChat[0].msg, msg);

	mChat[0].color = clr;
}

void SendChatMessage()
{
	nm.SendPlayerChat();
	enterChat = -1;
	enterMsg[0] = 0;
}