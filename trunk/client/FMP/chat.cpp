#include <windows.h>
#include <stdio.h>

#include "log.h"
#include "main.h"
#include "..\..\Shared\Network\NetworkProtocol.h"
// RakNet
#include "..\..\Shared\RakNet\RakNetworkFactory.h"
#include "..\..\Shared\RakNet\RakPeerInterface.h"
#include "..\..\Shared\RakNet\MessageIdentifiers.h"
#include "..\..\Shared\RakNet\BitStream.h"
#include "..\..\Shared\RakNet\NetworkIDObject.h"
#include "..\..\Shared\RakNet\NetworkIDManager.h"

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
	NetworkPlayerChatData data;
	strcpy(data.msg, enterMsg);
	RakNet::BitStream bsSend;
	bsSend.Write(data);
	net->RPC("RPC_Chat",&bsSend,HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

	enterChat = -1;
	enterMsg[0] = 0;
}