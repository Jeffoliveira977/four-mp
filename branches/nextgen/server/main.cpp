#include <stdio.h>
#include "network/CNetwork.h"
#include "main.h"
#include "log/log.h"
#include "log/logFile.h"
#include "log/logStd.h"

LogFile logfile;
LogStd logstd;

CNetwork * pNetwork;

int main(int argc, char *argv[])
{
	Log::AddLogger(&logfile);
	Log::AddLogger(&logstd);
	Log::Void("FOUR-MP Server %s loaded", FMP_VERSION);

	// Initialize
	pNetwork = new CNetwork;

	// Work
	while(pNetwork->IsReady())
	{
		pNetwork->Tick();
		Sleep(33);
	}

	// Uninitialize
	delete pNetwork;
	return 0;
}