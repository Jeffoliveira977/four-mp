// ConsoleClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ClientCore.h"
#include "../../Shared/Console/ConsoleCore.h"
#include "../../Shared/Console/ConsoleScreen.h"
#include "NetworkManager.h"

ClientCore client;
ConsoleCore concore;
ConsoleScreen conscreen(&concore);
NetworkManager nm;

int _tmain(int argc, _TCHAR* argv[])
{
	if (!client.Load())
	{
		return 1;
	}
	while (client.IsRunning())
	{
		client.Tick();
	}
	client.Unload();
	return 0;
}

