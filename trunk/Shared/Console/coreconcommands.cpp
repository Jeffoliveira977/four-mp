/// \file
/// \brief Source file that contains implementation of the core console commands.
/// \details See coreconcommands.h.
/// \author FaTony

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

#include "common.h"
#include "coreconcommands.h"

using namespace std;

void ConCmdAlias(ConsoleCore *concore, const unsigned char numargs)
{
	if (numargs == 0)
	{
		concore->Output("Current alias commands:");
		char *aliasstring;
		char *tempstring;
		for (unsigned short i = 0; i < concore->symbolbuffersize; i++)
		{
			if (concore->symbolbuffer[i].type == ConsoleCore::ConsoleSymbolTypeConAlias)
			{
				tempstring = concore->symbolbuffer[i].ptr->conalias->GetName();
				aliasstring = (char *)calloc(_scprintf("%s :", tempstring) + 1, sizeof(char));
				sprintf(aliasstring, "%s :", tempstring);
				free(tempstring);
				tempstring = concore->symbolbuffer[i].ptr->conalias->GetDescription();
				ResizeBuffer<char *>(aliasstring, _scprintf("%s %s", aliasstring, tempstring) + 1);
				sprintf(aliasstring, "%s %s", aliasstring, tempstring);
				free(tempstring);
				concore->Output(aliasstring);
				free(aliasstring);
			}
		}
		return;
	}
	char *name;
	if (!concore->GetCmdArg(1, name))
	{
		return;
	}
	char *commandstring;
	if (!concore->GetCmdArg(2, commandstring))
	{
		concore->AddConAlias(name);
	}
	concore->AddConAlias(name, commandstring);
}

void ConCmdCvarlist(ConsoleCore *concore, const unsigned char numargs)
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
	unsigned short numfound = 0;
	unsigned char numcmds;
	char *symbolstring;
	char *tempstring;
	for (unsigned short i = 0; i < concore->symbolbuffersize; i++)
	{
		//TODO: Sort alphabetically
		if (concore->symbolbuffer[i].type == ConsoleCore::ConsoleSymbolTypeConCmd)
		{
			numcmds = concore->symbolbuffer[i].numcmds;
		}
		else
		{
			numcmds = 1;
		}
		for (unsigned char j = 0; j < numcmds; j++)
		{
			symbolstring = (char *)calloc(strlen(concore->symbolbuffer[i].name) + 3, sizeof(char));
			sprintf(symbolstring, "%s	:", concore->symbolbuffer[i].name);
			switch (concore->symbolbuffer[i].type)
			{
			case ConsoleCore::ConsoleSymbolTypeConVar:
				{
					concore->symbolbuffer[i].ptr->convar->GetValue(tempstring);
					ResizeBuffer<char *>(symbolstring, _scprintf("%s %s	:", symbolstring, tempstring) + 1);
					sprintf(symbolstring, "%s %s	:", symbolstring, tempstring);
					//TODO: flags
					ResizeBuffer<char *>(symbolstring, strlen(symbolstring) + 3);
					sprintf(symbolstring, "%s	:", symbolstring);
					//TODO: flags
					free(tempstring);
					tempstring = concore->symbolbuffer[i].ptr->convar->GetDescription();
					ResizeBuffer<char *>(symbolstring, _scprintf("%s %s", symbolstring, tempstring) + 1);
					sprintf(symbolstring, "%s %s", symbolstring, tempstring);
					free(tempstring);
					concore->Output(symbolstring);
					break;
				}
			case ConsoleCore::ConsoleSymbolTypeConCmd:
				{
					ResizeBuffer<char *>(symbolstring, strlen(symbolstring) + 7);
					sprintf(symbolstring, "%s cmd	:", symbolstring);
					//TODO: flags
					ResizeBuffer<char *>(symbolstring, strlen(symbolstring) + 3);
					sprintf(symbolstring, "%s	:", symbolstring);
					//TODO: flags
					//free(tempstring);
					tempstring = concore->symbolbuffer[i].ptr[j].concmd->GetDescription();
					ResizeBuffer<char *>(symbolstring, _scprintf("%s %s", symbolstring, tempstring) + 1);
					sprintf(symbolstring, "%s %s", symbolstring, tempstring);
					free(tempstring);
					concore->Output(symbolstring);
					break;
				}
			case ConsoleCore::ConsoleSymbolTypeConAlias:
				{
					break;
				}
			}
			
			free(symbolstring);
		}
	}
	concore->Output("--------------\n%d total convars/concommands", concore->symbolbuffersize);
}

void ConCmdEcho(ConsoleCore *concore, const unsigned char numargs)
{
	char *tempstring = concore->GetCmdArgString();
	if (tempstring == NULL)
	{
		return;
	}
	concore->Output(tempstring);
	free(tempstring);
}

void ConCmdExec(ConsoleCore *concore, const unsigned char numargs)
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
	unsigned int length = strlen(tempstring);
	unsigned int i = 0;
	while ((tempstring[i] != '.') && (i < length))
	{
		i++;
	}
	char *filename;
	if (i == length)
	{
		length = length + 4;
		filename = (char *)calloc(length + 1, sizeof(char));
		sprintf(filename, "%s.cfg", tempstring);
	}
	else
	{
		filename = (char *)calloc(length + 1, sizeof(char));
		strcpy(filename, tempstring);
	}
	char *fullpath;
	if (concore->execpath != NULL)
	{
		fullpath = (char *)calloc(_scprintf("%s%s", concore->execpath, filename) + 1, sizeof(char));
		sprintf(fullpath, "%s%s", concore->execpath, filename);
	}
	else
	{
		fullpath = (char *)calloc(length + 1, sizeof(char));
		strcpy(fullpath, filename);
	}
	fstream execfile;
	execfile.open(fullpath, ios::in);
	if (!execfile.is_open())
	{
		concore->Output("Couldn't exec %s", tempstring);
		free(tempstring);
		free(fullpath);
		return;
	}
	string execstring;
	while (getline(execfile, execstring, '\n'))
	{
		concore->InterpretLine(execstring.c_str());
	}
	free(fullpath);
	free(tempstring);
}

void ConCmdFind(ConsoleCore *concore, const unsigned char numargs)
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

void ConCmdHelp(ConsoleCore *concore, const unsigned char numargs)
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