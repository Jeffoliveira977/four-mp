#include <windows.h>
#include <stdio.h>

#include "log.h"
#include "main.h"
// RakNet
#include "net\RakNetworkFactory.h"
#include "net\RakPeerInterface.h"
#include "net\MessageIdentifiers.h"
#include "net\BitStream.h"
#include "net\NetworkIDObject.h"
#include "net\NetworkIDManager.h"

extern RakPeerInterface *net;

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
	RakNet::BitStream bsSend;
	bsSend.Write(enterChat);
	bsSend.Write(enterMsg);
	net->RPC("Chat",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

	enterChat = -1;
	enterMsg[0] = 0;
}