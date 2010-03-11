#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "coreconcommands.h"
#include "ConsoleCore.h"

extern ConsoleCore concore;

void ConCmdCvarlist(unsigned char numargs)
{
	bool search = false;
	char *searchstring;
	if (numargs > 0)
	{
		if (concore.GetCmdArg(1, searchstring))
		{
			search = true;
		}
	}
	concore.Output("cvar list\n--------------");
	unsigned short numsymbols = concore.GetNumberOfConsoleSymbols();
	unsigned short numfound = 0;
	ConsoleCore::ConsoleSymbol *symbol;
	char *symbolstring;
	char *tempstring;
	for (unsigned short i = 0; i < numsymbols; i++)
	{
		//TODO: Sort alphabetically
		symbol = concore.GetConsoleSymbolByIndex(i);
		symbolstring = (char *)calloc(strlen(symbol->name) + 3, sizeof(char));
		sprintf(symbolstring, "%s	:", symbol->name);
		switch (symbol->type)
		{
		case ConsoleCore::ConsoleSymbolTypeConVar:
			{
				symbol->ptr.convar->GetValue(tempstring);
				ResizeStringBuffer(symbolstring, _scprintf("%s %s	:", symbolstring, tempstring) + 1);
				sprintf(symbolstring, "%s %s	:", symbolstring, tempstring);
				//TODO: flags
				ResizeStringBuffer(symbolstring, strlen(symbolstring) + 3);
				sprintf(symbolstring, "%s	:", symbolstring);
				//TODO: flags
				free(tempstring);
				tempstring = symbol->ptr.convar->GetDescription();
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
				tempstring = symbol->ptr.concmd->GetDescription();
				ResizeStringBuffer(symbolstring, _scprintf("%s %s", symbolstring, tempstring) + 1);
				sprintf(symbolstring, "%s %s", symbolstring, tempstring);
				free(tempstring);
				break;
			}
		}
		concore.Output(symbolstring);
		free(symbolstring);
	}
	concore.Output("--------------\n%d total convars/concommands", numsymbols);
}

void ConCmdFind(unsigned char numargs)
{
	if (numargs != 1)
	{
		concore.Output("Usage: find <string>");
		return;
	}
	char *searchstring;
	if (!concore.GetCmdArgString(searchstring))
	{
		concore.Output("Usage: find <string>");
		return;
	}
	unsigned short numsymbols = concore.GetNumberOfConsoleSymbols();
	char *tempstring;
	for (unsigned short i = 0; i < numsymbols; i++)
	{
		//TODO: Sort alphabetically
		tempstring = concore.GetConsoleSymbolHelpStringByIndex(i);
		if (strstr(tempstring, searchstring) != NULL)
		{
			concore.Output(tempstring);
		}
		free(tempstring);
	}
}

void ConCmdHelp(unsigned char numargs)
{
	if (numargs != 1)
	{
		concore.Output("Usage: help <cvarname>");
		return;
	}
	char *cvarname;
	if (!concore.GetCmdArg(1, cvarname))
	{
		concore.Output("Usage: help <cvarname>");
		free(cvarname);
		return;
	}
	if (!concore.IsConsoleSymbolExist(cvarname))
	{
		concore.Output("help: no cvar or command named %s", cvarname);
		free(cvarname);
		return;
	}
	concore.Output(concore.GetConsoleSymbolHelpString(cvarname));
	free(cvarname);
}