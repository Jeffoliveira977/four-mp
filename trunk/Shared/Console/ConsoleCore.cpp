/// \file
/// \brief Source file that contains implementation of the ConsoleCore class.
/// \details See class description.
/// \author FaTony

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "common.h"
#include "ConsoleCore.h"
#include "coreconcommands.h"

ConsoleCore::ConsoleCore(void)
{
	outputfunction = NULL;
	execpath = NULL;
	maxsymbolbuffersize = 65535;
	symbolbuffersize = 0;
	maxhookspercvar = 255;
	maxcmdspersymbol = 255;
	argpos = 0;
	numargs = 0;
}

ConsoleCore::~ConsoleCore(void)
{
	unsigned short i;
	for (i = 0; i < symbolbuffersize; i++)
	{
		switch (symbolbuffer[i].type)
		{
		case ConsoleSymbolTypeConVar:
			{
				delete symbolbuffer[i].ptr->convar;
				break;
			}
		case ConsoleSymbolTypeConCmd:
			{
				for (unsigned char j = 0; j < symbolbuffer[i].numcmds; j++)
				{
					delete symbolbuffer[i].ptr[0].concmd;
				}
				break;
			}
		case ConsoleSymbolTypeConAlias:
			{
				delete symbolbuffer[i].ptr->conalias;
				break;
			}
		}
	}
	free(symbolbuffer);
	if (commandbuffer != NULL)
	{
		free(commandbuffer);
	}
	if (commandargs != NULL)
	{
		for (i = 0; i <= numargs; i++)
		{
			free(commandargs[i]);
		}
	}
	free(commandargs);
}

void ConsoleCore::SetOutputFunction(void *function)
{
	outputfunction = function;
}

void ConsoleCore::SetExecPath(const char *string)
{
	if (string == NULL)
	{
		if (execpath != NULL)
		{
			free(execpath);
			execpath = NULL;
		}
		return;
	}
	execpath = (char *)calloc(strlen(string) + 1, sizeof(char));
	strcpy(execpath, string);
}

void ConsoleCore::RegisterStandardLibrary(void)
{
	this->AddConCmd("alias", ConCmdAlias, "Alias a command.", 0);
	this->AddConCmd("cvarlist", ConCmdCvarlist, "Show the list of convars/concommands.", 0);
	this->AddConVar("developer", 0, "Show developer messages.", 0, true, 0, true, 2);
	this->AddConCmd("echo", ConCmdEcho, "Echo text to console.", 0);
	this->AddConCmd("exec", ConCmdExec, "Execute script file.", 0);
	this->AddConCmd("find", ConCmdFind, "Find concommands with the specified string in their name/help text.", 0);
	this->AddConCmd("help", ConCmdHelp, "Find help about a convar/concommand.", 0);
}

void ConsoleCore::Output(const char *string, ...)
{
	if (outputfunction == NULL)
	{
		return;
	}
	va_list arglist; 
    va_start(arglist, string);
	int stringsize = _vscprintf(string, arglist);
	char *tempstring = (char *)calloc(stringsize + 1, sizeof(char));
	vsprintf(tempstring, string, arglist);
	va_end(arglist);
	((void (*) (const char *))outputfunction)(tempstring);
	free(tempstring);
}


unsigned short ConsoleCore::GetNumberOfConsoleSymbols(void)
{
	return symbolbuffersize;
}

bool ConsoleCore::IsConsoleSymbolExist(const char *name)
{
	if (name == NULL)
	{
		return false;
	}
	for (unsigned short i = 0; i < symbolbuffersize; i++)
	{
		if ((strcmp(symbolbuffer[i].name, name)) == 0)
		{
			return true;
		}
	}
	return false;
}

ConVar *ConsoleCore::AddConVar(const char *name, const float defaultvalue, const char *description, const int flags, const bool hasMin, const float min, const bool hasMax, const float max)
{
	if (name == NULL)
	{
		return NULL;
	}
	if (hasMin && hasMax && (min > max))
	{
		return NULL;
	}
	if (symbolbuffersize == maxsymbolbuffersize)
	{
		return NULL;
	}
	if (this->IsConsoleSymbolExist(name))
	{
		return NULL;
	}
	if (!ResizeBuffer<ConsoleSymbol *, ConsoleSymbol, unsigned short>(symbolbuffer, symbolbuffersize + 1))
	{
		return NULL;
	}
	symbolbuffer[symbolbuffersize].name = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(symbolbuffer[symbolbuffersize].name, name);
	symbolbuffer[symbolbuffersize].type = ConsoleSymbolTypeConVar;
	symbolbuffer[symbolbuffersize].ptr = new ConsoleSymbolPtr;
	symbolbuffer[symbolbuffersize].ptr->convar = new ConVar(this, name, defaultvalue, description, flags, hasMin, min, hasMax, max);
	symbolbuffer[symbolbuffersize].numcmds = 0;
	symbolbuffersize++;
	return symbolbuffer[symbolbuffersize-1].ptr->convar;
}

ConVar *ConsoleCore::AddConVar(const char *name, const int defaultvalue, const char *description, const int flags, const bool hasMin, const int min, const bool hasMax, const int max)
{
	if (name == NULL)
	{
		return NULL;
	}
	if (hasMin && hasMax && (min > max))
	{
		return NULL;
	}
	if (symbolbuffersize == maxsymbolbuffersize)
	{
		return NULL;
	}
	if (this->IsConsoleSymbolExist(name))
	{
		return NULL;
	}
	if (!ResizeBuffer<ConsoleSymbol *, ConsoleSymbol, unsigned short>(symbolbuffer, symbolbuffersize + 1))
	{
		return NULL;
	}
	symbolbuffer[symbolbuffersize].name = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(symbolbuffer[symbolbuffersize].name, name);
	symbolbuffer[symbolbuffersize].type = ConsoleSymbolTypeConVar;
	symbolbuffer[symbolbuffersize].ptr = new ConsoleSymbolPtr;
	symbolbuffer[symbolbuffersize].ptr->convar = new ConVar(this, name, defaultvalue, description, flags, hasMin, min, hasMax, max);
	symbolbuffer[symbolbuffersize].numcmds = 0;
	symbolbuffersize++;
	return symbolbuffer[symbolbuffersize-1].ptr->convar;
}

ConVar *ConsoleCore::AddConVar(const char *name, const char *defaultvalue, const char *description, const int flags)
{
	if (name == NULL)
	{
		return NULL;
	}
	if (symbolbuffersize == maxsymbolbuffersize)
	{
		return NULL;
	}
	if (this->IsConsoleSymbolExist(name))
	{
		return NULL;
	}
	if (!ResizeBuffer<ConsoleSymbol *, ConsoleSymbol, unsigned short>(symbolbuffer, symbolbuffersize + 1))
	{
		return NULL;
	}
	symbolbuffer[symbolbuffersize].name = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(symbolbuffer[symbolbuffersize].name, name);
	symbolbuffer[symbolbuffersize].type = ConsoleSymbolTypeConVar;
	symbolbuffer[symbolbuffersize].ptr = new ConsoleSymbolPtr;
	symbolbuffer[symbolbuffersize].ptr->convar = new ConVar(this, name, defaultvalue, description, flags);
	symbolbuffer[symbolbuffersize].numcmds = 0;
	symbolbuffersize++;
	return symbolbuffer[symbolbuffersize-1].ptr->convar;
}

ConCmd *ConsoleCore::AddConCmd(const char *name, void *callback, const char *description, const int flags)
{
	if (name == NULL)
	{
		return NULL;
	}
	if (callback == NULL)
	{
		return NULL;
	}
	unsigned short index;
	if (this->GetConsoleSymbolIndex(name, index))
	{
		if (symbolbuffer[index].numcmds == maxcmdspersymbol)
		{
			return NULL;
		}
		if (!ResizeBuffer<ConsoleSymbolPtr *, ConsoleSymbolPtr, unsigned char>(symbolbuffer[index].ptr, symbolbuffer[index].numcmds + 1))
		{
			return NULL;
		}
		symbolbuffer[index].ptr[symbolbuffer[index].numcmds].concmd = new ConCmd(this, name, callback, description, flags);
		symbolbuffer[index].numcmds++;
		return symbolbuffer[index].ptr[symbolbuffer[index].numcmds-1].concmd;
	}
	if (symbolbuffersize == maxsymbolbuffersize)
	{
		return NULL;
	}
	if (!ResizeBuffer<ConsoleSymbol *, ConsoleSymbol, unsigned short>(symbolbuffer, symbolbuffersize + 1))
	{
		return NULL;
	}
	symbolbuffer[symbolbuffersize].name = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(symbolbuffer[symbolbuffersize].name, name);
	symbolbuffer[symbolbuffersize].type = ConsoleSymbolTypeConCmd;
	symbolbuffer[symbolbuffersize].ptr = new ConsoleSymbolPtr;
	symbolbuffer[symbolbuffersize].ptr[0].concmd = new ConCmd(this, name, callback, description, flags);
	symbolbuffer[symbolbuffersize].numcmds = 1;
	symbolbuffersize++;
	return symbolbuffer[symbolbuffersize-1].ptr[0].concmd;
}

char *ConsoleCore::GetConsoleSymbolHelpString(const char *name)
{
	if (name == NULL)
	{
		return NULL;
	}
	unsigned short index;
	if (!this->GetConsoleSymbolIndex(name, index))
	{
		return NULL;
	}
	return this->GetHelpString(&symbolbuffer[index]);
}

char *ConsoleCore::GetConsoleSymbolHelpStringByIndex(unsigned short index)
{
	if (index >= symbolbuffersize)
	{
		return NULL;
	}
	return this->GetHelpString(&symbolbuffer[index]);
}

unsigned char ConsoleCore::GetCmdArgs(void)
{
	return numargs;
}

char *ConsoleCore::GetCmdArgString(void)
{
	if ((argpos == 0) || (commandbuffer == NULL))
	{
		return NULL;
	}
	char *arg = (char *)calloc(strlen(commandbuffer + argpos) + 1, sizeof(char));
	strcpy(arg, commandbuffer + argpos);
	return arg;
}

bool ConsoleCore::GetCmdArgType(const unsigned char argnum, ConVarType &type)
{
	if ((argpos == 0) || (commandbuffer == NULL))
	{
		return false;
	}
	if (!((commandargs[argnum][0] == '-') || ((commandargs[argnum][0] >= 48) && (commandargs[argnum][0] <= 57))))
	{
		type = ConVarTypeString;
		return true;
	}
	unsigned char firstnum = 0;
	if (commandargs[argnum][0] == '-')
	{
		firstnum = 1;
	}
	unsigned int length = strlen(commandargs[argnum]);
	unsigned int i = 1;
	bool floatflag = false;
	while (i < length)
	{
		if (!((commandargs[argnum][i] == '.') || ((commandargs[argnum][i] >= 48) && (commandargs[argnum][i] <= 57))))
		{
			type = ConVarTypeString;
			return true;
		}
		if (commandargs[argnum][i] == '.')
		{
			if (floatflag)
			{
				type = ConVarTypeString;
				return true;
			}
			floatflag = true;
			if (i == firstnum)
			{
				type = ConVarTypeString;
				return true;
			}
		}
		i++;
	}
	if (floatflag)
	{
		type = ConVarTypeFloat;
		return true;
	}
	type = ConVarTypeInt;
	return true;
}

bool ConsoleCore::GetCmdArg(const unsigned char argnum, char *&arg)
{
	if (argnum > numargs)
	{
		return false;
	}
	arg = (char *)calloc(strlen(commandargs[argnum]) + 1, sizeof(char));
	strcpy(arg, commandargs[argnum]);
	return true;
}

bool ConsoleCore::GetCmdArg(const unsigned char argnum, int &arg)
{
	if (argnum > numargs)
	{
		return false;
	}
	ConVarType type;
	if ((this->GetCmdArgType(argnum, type)) && (type != ConVarTypeInt))
	{
		return false;
	}
	arg = atoi(commandargs[argnum]);
	return true;
}

bool ConsoleCore::GetCmdArg(const unsigned char argnum, float &arg)
{
	if (argnum > numargs)
	{
		return false;
	}
	ConVarType type;
	if ((this->GetCmdArgType(argnum, type)) && (type != ConVarTypeFloat))
	{
		return false;
	}
	arg = (float)atof(commandargs[argnum]);
	return true;
}

void ConsoleCore::InterpretLine(const char *string)
{
	unsigned int length = strlen(string);
	unsigned int i = 0;
	while (((string[i] == ' ') || (string[i] == '"')) && (i < length))
	{
		i++;
	}
	if (i == length)
	{
		return;
	}
	if ((string[i] == '/') && (string[i+1] == '/'))
	{
		return;
	}
	bool comment;
	while (i < length)
	{
		commandbuffer = this->GetCommand(string, i);
		i = i + strlen(commandbuffer) + 1;
		comment = (!this->InterpretCommand());
		free(commandbuffer);
		commandbuffer = NULL;
		if (comment)
		{
			return;
		}
	}
}

void ConsoleCore::AddConAlias(const char *name, const char *cmdstring)
{
	if (name == NULL)
	{
		return;
	}
	if (cmdstring == NULL)
	{
		return;
	}
	unsigned short index;
	if (this->GetConsoleSymbolIndex(name, index))
	{
		if (symbolbuffer[index].type != ConsoleSymbolTypeConAlias)
		{
			this->Output("Cannot redefine registered convars/concommands.");
			return;
		}
		symbolbuffer[index].ptr->conalias->SetCommandString(cmdstring);
		return;
	}
	if (symbolbuffersize == maxsymbolbuffersize)
	{
		return;
	}
	if (!ResizeBuffer<ConsoleSymbol *, ConsoleSymbol, unsigned short>(symbolbuffer, symbolbuffersize + 1))
	{
		return;
	}
	symbolbuffer[symbolbuffersize].name = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(symbolbuffer[symbolbuffersize].name, name);
	symbolbuffer[symbolbuffersize].type = ConsoleSymbolTypeConAlias;
	symbolbuffer[symbolbuffersize].ptr = new ConsoleSymbolPtr;
	symbolbuffer[symbolbuffersize].ptr->conalias = new ConAlias(this, name, cmdstring);
	symbolbuffer[symbolbuffersize].numcmds = 0;
	symbolbuffersize++;
	return;
}

bool ConsoleCore::DeleteConsoleSymbolByIndex(const unsigned short index)
{
	if (index >= symbolbuffersize)
	{
		return false;
	}
	free(symbolbuffer[index].name);
	for (unsigned short i = index; i < (symbolbuffersize - 1); i++)
	{
		symbolbuffer[i] = symbolbuffer[i+1];
	}
	
	if (!ResizeBuffer<ConsoleSymbol *, ConsoleSymbol, unsigned short>(symbolbuffer, symbolbuffersize - 1))
	{
		return false;
	}
	symbolbuffersize--;
	return true;
}

void ConsoleCore::DeleteConVar(const char *name)
{
	if (name == NULL)
	{
		return;
	}
	unsigned short index;
	if (!this->GetConsoleSymbolIndex(name, index))
	{
		return;
	}
	if (symbolbuffer[index].type != ConsoleSymbolTypeConVar)
	{
		return;
	}
	if (!this->DeleteConsoleSymbolByIndex(index))
	{
		return;
	}
}

void ConsoleCore::DeleteConCmd(const char *name, ConCmd *ptr)
{
	if (name == NULL)
	{
		return;
	}
	if (ptr == NULL)
	{
		return;
	}
	unsigned short index;
	if (!this->GetConsoleSymbolIndex(name, index))
	{
		return;
	}
	if (symbolbuffer[index].type != ConsoleSymbolTypeConCmd)
	{
		return;
	}
	if (symbolbuffer[index].numcmds == 1)
	{
		if (!this->DeleteConsoleSymbolByIndex(index))
		{
			return;
		}
		return;
	}
	for (unsigned char i = 0; i < symbolbuffer[index].numcmds; i++)
	{
		if (symbolbuffer[index].ptr[i].concmd == ptr)
		{
			for (; i < (symbolbuffer[index].numcmds - 1); i++)
			{
				symbolbuffer[index].ptr[i] = symbolbuffer[index].ptr[i+1];
			}
			if (!ResizeBuffer<ConsoleSymbolPtr *, ConsoleSymbolPtr, unsigned char>(symbolbuffer[index].ptr, symbolbuffer[index].numcmds - 1))
			{
				return;
			}
			symbolbuffer[index].numcmds--;
			return;
		}
	}
}

bool ConsoleCore::GetConsoleSymbolIndex(const char *name, unsigned short &index)
{
	for (index = 0; index < symbolbuffersize; index++)
	{
		if ((strcmp(symbolbuffer[index].name, name)) == 0)
		{
			return true;
		}
	}
	return false;
}

char *ConsoleCore::GetHelpString(const ConsoleSymbol *symbol)
{
	char *helpstring = (char *)calloc(strlen(symbol->name) + 3, sizeof(char));
	sprintf(helpstring, "\"%s\"", symbol->name);
	switch (symbol->type)
	{
	case ConsoleSymbolTypeConVar:
		{
			switch (symbol->ptr->convar->GetType())
			{
			case ConVarTypeFloat:
				{
					float value;
					symbol->ptr->convar->GetValue(value);
					ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s = \"%f\"", helpstring, value) + 1);
					sprintf(helpstring, "%s = \"%f\"", helpstring, value);
					break;
				}
			case ConVarTypeInt:
				{
					int value;
					symbol->ptr->convar->GetValue(value);
					ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s = \"%d\"", helpstring, value) + 1);
					sprintf(helpstring, "%s = \"%d\"", helpstring, value);
					break;
				}
			case ConVarTypeString:
				{
					char *value;
					symbol->ptr->convar->GetValue(value);
					ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s = \"%s\"", helpstring, value) + 1);
					sprintf(helpstring, "%s = \"%s\"", helpstring, value);
					free(value);
					break;
				}
			}
			switch (symbol->ptr->convar->GetDefaultType())
			{
			case ConVarTypeFloat:
				{
					float value;
					symbol->ptr->convar->GetDefaultValue(value);
					ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s ( def. \"%f\" )", helpstring, value) + 1);
					sprintf(helpstring, "%s ( def. \"%f\" )", helpstring, value);
					if (symbol->ptr->convar->GetBound(ConVarBoundLower, value))
					{
						ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s min. %f", helpstring, value) + 1);
						sprintf(helpstring, "%s min. %f", helpstring, value);
					}
					if (symbol->ptr->convar->GetBound(ConVarBoundUpper, value))
					{
						ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s max. %f", helpstring, value) + 1);
						sprintf(helpstring, "%s max. %f", helpstring, value);
					}
					break;
				}
			case ConVarTypeInt:
				{
					int value;
					symbol->ptr->convar->GetDefaultValue(value);
					ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s ( def. \"%d\" )", helpstring, value) + 1);
					sprintf(helpstring, "%s ( def. \"%d\" )", helpstring, value);
					if (symbol->ptr->convar->GetBound(ConVarBoundLower, value))
					{
						ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s min. %d", helpstring, value) + 1);
						sprintf(helpstring, "%s min. %d", helpstring, value);
					}
					if (symbol->ptr->convar->GetBound(ConVarBoundUpper, value))
					{
						ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s max. %d", helpstring, value) + 1);
						sprintf(helpstring, "%s max. %d", helpstring, value);
					}
					break;
				}
			case ConVarTypeString:
				{
					char *value;
					symbol->ptr->convar->GetDefaultValue(value);
					ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s  ( def. \"%s\" )", helpstring, value) + 1);
					sprintf(helpstring, "%s  ( def. \"%s\" )", helpstring, value);
					free(value);
					break;
				}
			}
			if (symbol->ptr->convar->GetFlags() != 0)
			{
				//TODO: flags
			}
			char *desc = symbol->ptr->convar->GetDescription();
			if (strlen(desc) != 0)
			{
				ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s\n - %s", helpstring, desc) + 1);
				sprintf(helpstring, "%s\n - %s", helpstring, desc);
			}
			free(desc);
			break;
		}
	case ConsoleSymbolTypeConCmd:
		{
			char *desc;
			for (unsigned char i = 0; i < symbol->numcmds; i++)
			{
				if (symbol->ptr->convar->GetFlags() != 0)
				{
					//TODO: flags
				}
				desc = symbol->ptr->convar->GetDescription();
				if (strlen(desc) != 0)
				{
					ResizeBuffer<char *, char, unsigned int>(helpstring, _scprintf("%s\n - %s", helpstring, desc) + 1);
					sprintf(helpstring, "%s\n - %s", helpstring, desc);
				}
				free(desc);
			}
			break;
		}
	case ConsoleSymbolTypeConAlias:
		{
			ResizeBuffer<char *, char, unsigned int>(helpstring, 1);
			helpstring[0] = '\0';
			break;
		}
	}
	return helpstring;
}

char *ConsoleCore::GetCommand(const char *string, const unsigned int startindex)
{
	unsigned int length = strlen(string);
	unsigned int i = startindex;
	bool stringargument = false;
	while ((!((string[i] == ';') && (stringargument == false))) && (i < length))
	{
		if (string[i] == '"')
		{
			stringargument = !stringargument;
		}
		i++;
	}
	length = i - startindex;
	char *cmdstring = (char *)calloc(length + 1, sizeof(char));
	strncpy(cmdstring, string + startindex, length);
	cmdstring[length] = '\0';
	return cmdstring;
}

bool ConsoleCore::InterpretCommand(void)
{
	unsigned int length = strlen(commandbuffer);
	unsigned int i = 0;
	while (((commandbuffer[i] == ' ') || (commandbuffer[i] == '"')) && (i < length))
	{
		i++;
	}
	unsigned int startindex = i;
	while ((commandbuffer[i] != ' ') && (i < length))
	{
		i++;
	}
	unsigned int templength = i - startindex;
	if (templength == 0)
	{
		return true;
	}
	char *symbolname = (char *)calloc(templength + 1, sizeof(char));
	strncpy(symbolname, commandbuffer + startindex, templength);
	symbolname[templength] = '\0';
	unsigned short index;
	if (!this->GetConsoleSymbolIndex(symbolname, index))
	{
		this->Output("Unknown command \"%s\"", symbolname);
		free(symbolname);
		return true;
	}
	if (commandargs != NULL)
	{
		for (unsigned char k = 0; k <= numargs; k++)
		{
			free(commandargs[k]);
			commandargs[k] = NULL;
		}
		free(commandargs);
	}
	commandargs = (char **)calloc(1, sizeof(char *));
	commandargs[0] = (char *)calloc(templength + 1, sizeof(char));
	strcpy(commandargs[0], symbolname);
	i++;
	unsigned char j = 1;
	unsigned int endindex = 0;
	bool comment = false;
	while ((i < length) && (!comment))
	{
		if (commandbuffer[i] == ' ')
		{
			i++;
		}
		else if (commandbuffer[i] == '"')
		{
			startindex = i + 1;
			if (j == 1)
			{
				argpos = startindex;
			}
			i++;
			while ((commandbuffer[i] != '"') && (i < length))
			{
				i++;
			}
			templength = i - startindex;
			i++;
			ResizeBuffer<char **, char *, unsigned short>(commandargs, j + 1);
			commandargs[j] = (char *)calloc(templength + 1, sizeof(char));
			strncpy(commandargs[j], commandbuffer + startindex, templength);
			commandargs[j][templength] = '\0';
			j++;
		}
		else
		{
			startindex = i;
			if (j == 1)
			{
				if ((commandbuffer[i] == '/') && (commandbuffer[i+1] == '/'))
				{
					comment = true;
				}
				else
				{
					argpos = startindex;
				}
			}
			if (!comment)
			{
				i++;
				while ((commandbuffer[i] != ' ') && (i < length))
				{
					i++;
				}
				templength = i - startindex;
				ResizeBuffer<char **, char *, unsigned short>(commandargs, j + 1);
				commandargs[j] = (char *)calloc(templength + 1, sizeof(char));
				strncpy(commandargs[j], commandbuffer + startindex, templength);
				commandargs[j][templength] = '\0';
				j++;
			}
		}
	}
	numargs = j - 1;
	switch (symbolbuffer[index].type)
	{
	case ConsoleSymbolTypeConVar:
		{
			if (numargs == 0)
			{
				this->Output(this->GetHelpString(&symbolbuffer[index]));
			}
			else if (numargs == 1)
			{
				ConVarType type;
				this->GetCmdArgType(1, type);
				switch (type)
				{
				case ConVarTypeFloat:
					{
						float value;
						this->GetCmdArg(1, value);
						symbolbuffer[index].ptr->convar->SetValue(value);
						break;
					}
				case ConVarTypeInt:
					{
						int value;
						this->GetCmdArg(1, value);
						symbolbuffer[index].ptr->convar->SetValue(value);
						break;
					}
				case ConVarTypeString:
					{
						char *value;
						this->GetCmdArg(1, value);
						symbolbuffer[index].ptr->convar->SetValue(value);
						free(value);
						break;
					}
				}
			}
			else
			{
				char *arg = this->GetCmdArgString();
				symbolbuffer[index].ptr->convar->SetValue(arg);
			}
			break;
		}
	case ConsoleSymbolTypeConCmd:
		{
			for (j = 0; j < symbolbuffer[index].numcmds; j++)
			{
				symbolbuffer[index].ptr[j].concmd->Execute();
			}
			break;
		}
	case ConsoleSymbolTypeConAlias:
		{
			symbolbuffer[index].ptr->conalias->Execute();
			break;
		}
	}
	free(symbolname);
	if (commandargs != NULL)
	{
		for (i = 0; i <= numargs; i++)
		{
			free(commandargs[i]);
			commandargs[i] = NULL;
		}
		free(commandargs);
	}
	commandargs = NULL;
	argpos = 0;
	numargs = 0;
	return !comment;
}
