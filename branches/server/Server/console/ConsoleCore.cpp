#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "common.h"
#include "ConsoleCore.h"

ConsoleCore::ConsoleCore(void)
{
	maxsymbolbuffersize = 65535;
	symbolbuffersize = 0;
}

ConsoleCore::~ConsoleCore(void)
{
	unsigned short i;
	for (i = 0; i < symbolbuffersize; i++)
	{
		this->DeleteConsoleSymbol(symbolbuffer[i].name);
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

void ConsoleCore::Output(const char *string, ...)
{
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
	for (unsigned short i = 0; i < symbolbuffersize; i++)
	{
		if ((strcmp(symbolbuffer[i].name, name)) == 0)
		{
			return true;
		}
	}
	return false;
}

ConsoleSymbol *ConsoleCore::GetConsoleSymbolByIndex(unsigned short index)
{
	if (index >= symbolbuffersize)
	{
		return NULL;
	}
	ConsoleSymbol *symbol = (ConsoleSymbol *)calloc(1, sizeof(ConsoleSymbol));
	memcpy(symbol, &symbolbuffer[index], sizeof(ConsoleSymbol));
	return symbol;
}

bool ConsoleCore::AddConsoleSymbol(ConsoleSymbol *symbol)
{
	if (symbolbuffersize == maxsymbolbuffersize)
	{
		return false;
	}
	if (this->IsConsoleSymbolExist(symbol->name))
	{
		return false;
	}
	symbolbuffersize++;
	this->ResizeSymbolBuffer(symbolbuffer, symbolbuffersize);
	symbolbuffer[symbolbuffersize-1] = *symbol;
	return true;
}

bool ConsoleCore::DeleteConsoleSymbol(const char *name)
{
	unsigned short index;
	if (!(this->GetConsoleSymbolIndex(name, index)))
	{
		return false;
	}
	free(symbolbuffer[index].name);
	if (symbolbuffer[index].type = ConsoleSymbolTypeConVar)
	{
		delete(symbolbuffer[index].ptr.convar);
	}
	else
	{
		delete(symbolbuffer[index].ptr.concmd);
	}
	//free(symbolbuffer[index]);
	for (unsigned short i = index; i < (symbolbuffersize - 1); i++)
	{
		symbolbuffer[i] = symbolbuffer[i+1];
	}
	symbolbuffersize--;
	this->ResizeSymbolBuffer(symbolbuffer, symbolbuffersize);
	return true;
}

char *ConsoleCore::GetConsoleSymbolHelpString(const char *name)
{
	ConsoleSymbol *symbol = this->GetConsoleSymbol(name);
	if (symbol == NULL)
	{
		return NULL;
	}
	return this->GetHelpString(symbol);
}

char *ConsoleCore::GetConsoleSymbolHelpStringByIndex(unsigned short index)
{
	if (index >= symbolbuffersize)
	{
		return NULL;
	}
	ConsoleSymbol *symbol = this->GetConsoleSymbolByIndex(index);
	if (symbol == NULL)
	{
		return NULL;
	}
	return this->GetHelpString(symbol);
}

unsigned char ConsoleCore::GetCmdArgs(void)
{
	return numargs;
}

bool ConsoleCore::GetCmdArgString(char *&arg)
{
	if ((argpos == 0) || (commandbuffer == NULL))
	{
		return false;
	}
	arg = (char *)calloc(strlen(commandbuffer + argpos) + 1, sizeof(char));
	strcpy(arg, commandbuffer + argpos);
	return true;
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
	while (i < length)
	{
	commandbuffer = this->GetCommand(string, i);
	this->InterpretCommand();
	i = i + strlen(commandbuffer) + 1;
	free(commandbuffer);
	commandbuffer = NULL;
	}
}

bool ConsoleCore::ResizeSymbolBuffer(ConsoleSymbol *&buffer, const unsigned short size)
{
	ConsoleSymbol *tempbuffer = (ConsoleSymbol *)realloc(buffer, size * sizeof(ConsoleSymbol));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

ConsoleSymbol *ConsoleCore::GetConsoleSymbol(const char *name)
{
	for (unsigned short i = 0; i < symbolbuffersize; i++)
	{
		if ((strcmp(symbolbuffer[i].name, name)) == 0)
		{
			return &symbolbuffer[i];
		}
	}
	return NULL;
}

bool ConsoleCore::GetConsoleSymbolIndex(const char *name, unsigned short &index)
{
	for (unsigned short i = 0; i < symbolbuffersize; i++)
	{
		if ((strcmp(symbolbuffer[i].name, name)) == 0)
		{
			index = i;
			return true;
		}
	}
	return false;
}

char *ConsoleCore::GetHelpString(const ConsoleSymbol *symbol)
{
	char *helpstring = (char *)calloc(strlen(symbol->name) + 3, sizeof(char));
	sprintf(helpstring, "\"%s\"", symbol->name);
	if (symbol->type == ConsoleSymbolTypeConVar)
	{
		switch (symbol->ptr.convar->GetType())
		{
		case ConVarTypeFloat:
			{
				float value;
				symbol->ptr.convar->GetValue(value);
				ResizeStringBuffer(helpstring, _scprintf("%s = \"%f\"", helpstring, value) + 1);
				sprintf(helpstring, "%s = \"%f\"", helpstring, value);
				break;
			}
		case ConVarTypeInt:
			{
				int value;
				symbol->ptr.convar->GetValue(value);
				ResizeStringBuffer(helpstring, _scprintf("%s = \"%d\"", helpstring, value) + 1);
				sprintf(helpstring, "%s = \"%d\"", helpstring, value);
				break;
			}
		case ConVarTypeString:
			{
				char *value;
				symbol->ptr.convar->GetValue(value);
				ResizeStringBuffer(helpstring, _scprintf("%s = \"%s\"", helpstring, value) + 1);
				sprintf(helpstring, "%s = \"%s\"", helpstring, value);
				free(value);
				break;
			}
		}
		switch (symbol->ptr.convar->GetDefaultType())
		{
		case ConVarTypeFloat:
			{
				float value;
				symbol->ptr.convar->GetDefaultValue(value);
				ResizeStringBuffer(helpstring, _scprintf("%s ( def. \"%f\" )", helpstring, value) + 1);
				sprintf(helpstring, "%s ( def. \"%f\" )", helpstring, value);
				if (symbol->ptr.convar->GetBoundFloat(ConVarBoundLower, value))
				{
					ResizeStringBuffer(helpstring, _scprintf("%s min. %f", helpstring, value) + 1);
					sprintf(helpstring, "%s min. %f", helpstring, value);
				}
				if (symbol->ptr.convar->GetBoundFloat(ConVarBoundUpper, value))
				{
					ResizeStringBuffer(helpstring, _scprintf("%s max. %f", helpstring, value) + 1);
					sprintf(helpstring, "%s max. %f", helpstring, value);
				}
				break;
			}
		case ConVarTypeInt:
			{
				int value;
				symbol->ptr.convar->GetDefaultValue(value);
				ResizeStringBuffer(helpstring, _scprintf("%s ( def. \"%d\" )", helpstring, value) + 1);
				sprintf(helpstring, "%s ( def. \"%d\" )", helpstring, value);
				if (symbol->ptr.convar->GetBoundInt(ConVarBoundLower, value))
				{
					ResizeStringBuffer(helpstring, _scprintf("%s min. %d", helpstring, value) + 1);
					sprintf(helpstring, "%s min. %d", helpstring, value);
				}
				if (symbol->ptr.convar->GetBoundInt(ConVarBoundUpper, value))
				{
					ResizeStringBuffer(helpstring, _scprintf("%s max. %d", helpstring, value) + 1);
					sprintf(helpstring, "%s max. %d", helpstring, value);
				}
				break;
			}
		case ConVarTypeString:
			{
				char *value;
				symbol->ptr.convar->GetDefaultValue(value);
				ResizeStringBuffer(helpstring, _scprintf("%s  ( def. \"%s\" )", helpstring, value) + 1);
				sprintf(helpstring, "%s  ( def. \"%s\" )", helpstring, value);
				free(value);
				break;
			}
		}
		if (symbol->ptr.convar->GetFlags() != 0)
		{
			//TODO: flags
		}
		char *desc = symbol->ptr.convar->GetDescription();
		if (strlen(desc) != 0)
		{
			ResizeStringBuffer(helpstring, _scprintf("%s\n - %s", helpstring, desc) + 1);
			sprintf(helpstring, "%s\n - %s", helpstring, desc);
		}
		free(desc);
	}
	else
	{
		if (symbol->ptr.convar->GetFlags() != 0)
		{
			//TODO: flags
		}
		char *desc = symbol->ptr.convar->GetDescription();
		if (strlen(desc) != 0)
		{
			ResizeStringBuffer(helpstring, _scprintf("%s\n - %s", helpstring, desc) + 1);
			sprintf(helpstring, "%s\n - %s", helpstring, desc);
		}
		free(desc);
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

void ConsoleCore::InterpretCommand(void)
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
		return;
	}
	char *symbolname = (char *)calloc(templength + 1, sizeof(char));
	strncpy(symbolname, commandbuffer + startindex, templength);
	symbolname[templength] = '\0';
	ConsoleSymbol *symbol = this->GetConsoleSymbol(symbolname);
	if (symbol == NULL)
	{
		this->Output("Unknown command \"%s\"", symbolname);
		free(symbolname);
		return;
	}
	commandargs = (char **)calloc(1, sizeof(char *));
	commandargs[0] = (char *)calloc(templength + 1, sizeof(char));
	strcpy(commandargs[0], symbolname);
	i++;
	unsigned char j = 1;
	unsigned int endindex = 0;
	while (i < length)
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
			ResizeArrayBuffer(commandargs, j + 1);
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
				argpos = startindex;
			}
			i++;
			while ((commandbuffer[i] != ' ') && (i < length))
			{
				i++;
			}
			templength = i - startindex;
			ResizeArrayBuffer(commandargs, j + 1);
			commandargs[j] = (char *)calloc(templength + 1, sizeof(char));
			strncpy(commandargs[j], commandbuffer + startindex, templength);
			commandargs[j][templength] = '\0';
			j++;
		}
	}
	numargs = j - 1;
	switch (symbol->type)
	{
	case ConsoleSymbolTypeConVar:
		{
			if (numargs == 0)
			{
				this->Output(this->GetHelpString(symbol));
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
						symbol->ptr.convar->SetValue(value);
						break;
					}
				case ConVarTypeInt:
					{
						int value;
						this->GetCmdArg(1, value);
						symbol->ptr.convar->SetValue(value);
						break;
					}
				case ConVarTypeString:
					{
						char *value;
						this->GetCmdArg(1, value);
						symbol->ptr.convar->SetValue(value);
						free(value);
						break;
					}
				}
			}
			else
			{
				char *arg;
				this->GetCmdArgString(arg);
				symbol->ptr.convar->SetValue(arg);
			}
			break;
		}
	case ConsoleSymbolTypeConCmd:
		{
			symbol->ptr.concmd->Execute();
			break;
		}
	}
	free(symbolname);
	for (i = 0; i <= numargs; i++)
	{
		free(commandargs[i]);
		commandargs[i] = NULL;
	}
	free(commandargs);
	commandargs = NULL;
	argpos = 0;
	numargs = 0;
}
