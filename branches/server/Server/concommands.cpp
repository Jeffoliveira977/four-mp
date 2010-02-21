#include <stdio.h>

#include "common.h"
#include "concommands.h"
#include "Console.h"
#include "ScriptCommandHandler.h"

extern Console con;
extern ScriptCommandHandler cmdhandler;

void ConCmdCvarlist(unsigned char numargs)
{
	bool search = false;
	char *searchstring;
	if (numargs > 0)
	{
		if (con.GetCmdArg(1, searchstring))
		{
			search = true;
		}
	}
	con.Print("cvar list\n--------------");
	unsigned short numsymbols = con.GetNumberOfConsoleSymbols();
	unsigned short numfound = 0;
	ConsoleSymbol *symbol;
	char *symbolstring;
	char *tempstring;
	for (unsigned short i = 0; i < numsymbols; i++)
	{
		//TODO: Sort alphabetically
		symbol = con.GetConsoleSymbolByIndex(i);
		symbolstring = (char *)calloc(strlen(symbol->name) + 3, sizeof(char));
		sprintf(symbolstring, "%s	:", symbol->name);
		switch (symbol->type)
		{
		case ConsoleSymbolTypeConVar:
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
		case ConsoleSymbolTypeConCmd:
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
		con.Print(symbolstring);
		free(symbolstring);
	}
	con.Print("--------------\n%d total convars/concommands", numsymbols);
}

void ConCmdFind(unsigned char numargs)
{
	if (numargs != 1)
	{
		con.Print("Usage: find <string>");
		return;
	}
	char *searchstring;
	if (!con.GetCmdArgString(searchstring))
	{
		con.Print("Usage: find <string>");
		return;
	}
	unsigned short numsymbols = con.GetNumberOfConsoleSymbols();
	char *tempstring;
	for (unsigned short i = 0; i < numsymbols; i++)
	{
		//TODO: Sort alphabetically
		tempstring = con.GetConsoleSymbolHelpStringByIndex(i);
		if (strstr(tempstring, searchstring) != NULL)
		{
			con.Print(tempstring);
		}
		free(tempstring);
	}
}

void ConCmdHelp(unsigned char numargs)
{
	if (numargs != 1)
	{
		con.Print("Usage: help <cvarname>");
		return;
	}
	char *cvarname;
	if (!con.GetCmdArg(1, cvarname))
	{
		con.Print("Usage: help <cvarname>");
		free(cvarname);
		return;
	}
	if (!con.IsConsoleSymbolExist(cvarname))
	{
		con.Print("help: no cvar or command named %s", cvarname);
		free(cvarname);
		return;
	}
	con.Print(con.GetConsoleSymbolHelpString(cvarname));
	free(cvarname);
}

void ConCmdSquirrel(unsigned char numargs)
{
	char *cmdname;
	if (!con.GetCmdArg(0, cmdname))
	{
		return;
	}
	if (!cmdhandler.Execute(cmdname, numargs))
	{
		con.Print("Unknown command \"%s\"", cmdname);
	}
	free(cmdname);
}

void ConCmdTest(unsigned char numargs)
{
	con.Print("0	1	2	3	4	5	6	7	8	9	0\n");
	//con.Print("	2	3	4	5");
}