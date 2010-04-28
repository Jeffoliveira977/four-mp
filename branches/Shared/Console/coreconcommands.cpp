/// \file
/// \brief Source file that contains implementation of the core console commands.
/// \details See coreconcommands.h.
/// \author FaTony

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "coreconcommands.h"

void ConCmdCvarlist(ConsoleCore *concore, unsigned char numargs)
{
	bool search = false;
	char *searchstring;
	if (numargs > 0)
	{
		if (concore->GetCmdArg(1, searchstring))
		{
			search = true;
		}
	}
	concore->Output("cvar list\n--------------");
	unsigned short numsymbols = concore->GetNumberOfConsoleSymbols();
	unsigned short numfound = 0;
	ConsoleCore::ConsoleSymbol *symbol;
	unsigned char numcmds;
	char *symbolstring;
	char *tempstring;
	for (unsigned short i = 0; i < numsymbols; i++)
	{
		//TODO: Sort alphabetically
		symbol = concore->GetConsoleSymbolByIndex(i);
		if (symbol->type == ConsoleCore::ConsoleSymbolTypeConCmd)
		{
			numcmds = symbol->numcmds;
		}
		else
		{
			numcmds = 1;
		}
		for (unsigned char j = 0; j < numcmds; j++)
		{
			symbolstring = (char *)calloc(strlen(symbol->name) + 3, sizeof(char));
			sprintf(symbolstring, "%s	:", symbol->name);
			switch (symbol->type)
			{
			case ConsoleCore::ConsoleSymbolTypeConVar:
				{
					symbol->ptr->convar->GetValue(tempstring);
					ResizeStringBuffer(symbolstring, _scprintf("%s %s	:", symbolstring, tempstring) + 1);
					sprintf(symbolstring, "%s %s	:", symbolstring, tempstring);
					//TODO: flags
					ResizeStringBuffer(symbolstring, strlen(symbolstring) + 3);
					sprintf(symbolstring, "%s	:", symbolstring);
					//TODO: flags
					free(tempstring);
					tempstring = symbol->ptr->convar->GetDescription();
					ResizeStringBuffer(symbolstring, _scprintf("%s %s", symbolstring, tempstring) + 1);
					sprintf(symbolstring, "%s %s", symbolstring, tempstring);
					free(tempstring);
					break;
				}
			case ConsoleCore::ConsoleSymbolTypeConCmd:
				{
					ResizeStringBuffer(symbolstring, strlen(symbolstring) + 7);
					sprintf(symbolstring, "%s cmd	:", symbolstring);
					//TODO: flags
					ResizeStringBuffer(symbolstring, strlen(symbolstring) + 3);
					sprintf(symbolstring, "%s	:", symbolstring);
					//TODO: flags
					//free(tempstring);
					tempstring = symbol->ptr[j].concmd->GetDescription();
					ResizeStringBuffer(symbolstring, _scprintf("%s %s", symbolstring, tempstring) + 1);
					sprintf(symbolstring, "%s %s", symbolstring, tempstring);
					free(tempstring);
					break;
				}
			}
			concore->Output(symbolstring);
			free(symbolstring);
		}
	}
	concore->Output("--------------\n%d total convars/concommands", numsymbols);
}

void ConCmdEcho(ConsoleCore *concore, unsigned char numargs)
{
	char *tempstring = concore->GetCmdArgString();
	if (tempstring == NULL)
	{
		return;
	}
	concore->Output(tempstring);
	free(tempstring);
}

void ConCmdExec(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Usage: exec <filename>: execute a script file");
		return;
	}
	char *tempstring = concore->GetCmdArgString();
	if (tempstring == NULL)
	{
		concore->Output("Usage: exec <filename>: execute a script file");
		return;
	}
	if (concore->execpath != NULL)
	{

	}
	free(tempstring);
}

void ConCmdFind(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs != 1)
	{
		concore->Output("Usage: find <string>");
		return;
	}
	char *searchstring = concore->GetCmdArgString();
	if (searchstring == NULL)
	{
		concore->Output("Usage: find <string>");
		return;
	}
	unsigned short numsymbols = concore->GetNumberOfConsoleSymbols();
	char *tempstring;
	for (unsigned short i = 0; i < numsymbols; i++)
	{
		//TODO: Sort alphabetically
		tempstring = concore->GetConsoleSymbolHelpStringByIndex(i);
		if (strstr(tempstring, searchstring) != NULL)
		{
			concore->Output(tempstring);
		}
		free(tempstring);
	}
}

void ConCmdHelp(ConsoleCore *concore, unsigned char numargs)
{
	if (numargs != 1)
	{
		concore->Output("Usage: help <cvarname>");
		return;
	}
	char *cvarname;
	if (!concore->GetCmdArg(1, cvarname))
	{
		concore->Output("Usage: help <cvarname>");
		free(cvarname);
		return;
	}
	if (!concore->IsConsoleSymbolExist(cvarname))
	{
		concore->Output("help: no cvar or command named %s", cvarname);
		free(cvarname);
		return;
	}
	concore->Output(concore->GetConsoleSymbolHelpString(cvarname));
	free(cvarname);
}