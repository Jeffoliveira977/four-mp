/// \file
/// \brief Source file that contains implementation of the four-mp client shared console
/// commands.
/// \details See con_sharedcommands.h.
/// \author FaTony

#include <string.h>

#include "con_sharedcommands.h"
#include "../Shared/ClientCore.h"
#include "../Shared/NetworkManager.h"

extern ClientCore client;
extern NetworkManager nm;

void ConCmdConnect(ConsoleCore *concore, const unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: connect <server>");
		return;
	}
	char *address = concore->GetCmdArgString();
	if (address == NULL)
	{
		concore->Output("Usage: connect <server>");
		return;
	}
	char *portstart = strrchr(address, ':');
	if (portstart == NULL)
	{
		nm.ConnectToServer(address);
		free(address);
		return;
	}
	unsigned short port = 7777;
	sscanf(portstart, "%d", port);
	nm.ConnectToServer(address, port);
	free(address);
	return;
}

void ConCmdQuit(ConsoleCore *concore, const unsigned char numargs)
{
	client.Shutdown();
}