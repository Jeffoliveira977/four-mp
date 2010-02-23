#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdarg.h>

#include "common.h"
#include "Console.h"

Console::Console(void)
{
	maxoutputbuffersize = 1024;
	maxinputbuffersize = 255;
	maxsymbolbuffersize = 65535;
	pagesize = 23;
#ifdef WIN32
	outputhandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	outputbuffer = (char **)calloc(1, sizeof(char *));
	outputbuffersize = 0;
	inputbuffer = (char **)calloc(1, sizeof(char *));
	inputbuffer[0] = (char *)calloc(1, sizeof(char));
	inputbuffersize = 1;
	inputbufferposition[0] = 0;
	inputbufferposition[1] = 0;
	screenposition = 0;
	symbolbuffersize = 0;
	this->UpdateScreen();
}

Console::~Console(void)
{
	unsigned short i;
	for (i = 0; i < outputbuffersize; i++)
	{
		free(outputbuffer[i]);
	}
	free(outputbuffer);
	for (i = 0; i < inputbuffersize; i++)
	{
		free(inputbuffer[i]);
	}
	free(inputbuffer);
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

void Console::CheckUserInput(void)
{
	if (_kbhit())
		{
			int ch = _getch();
			switch (ch)
			{
			case 0: //Control characters
				{
					ch = _getch();
					switch (ch)
					{
					case 59: //F1
						{
							
							break;
						}
					case 60: //F2
						{
							
							break;
						}
					case 61: //F3
						{
							
							break;
						}
					case 62: //F4
						{
							
							break;
						}
					case 63: //F5
						{
							
							break;
						}
					case 64: //F6
						{
							
							break;
						}
					case 65: //F7
						{
							
							break;
						}
					case 66: //F8
						{
							
							break;
						}
					case 67: //F9
						{
							
							break;
						}
					case 68: //F10
						{
							
							break;
						}
					}
					//printf("Control: 0 %d\n", ch);
					break;
				}
			case 224: //Control characters
				{
					ch = _getch();
					switch (ch)
					{
					case 71: //Home
						{
							
							break;
						}
					case 72: //Up arrow
						{
							this->InputHistoryBack();
							break;
						}
					case 73: //Page up
						{
							this->GoOnePageUp();
							break;
						}
					case 75: //Left arrow
						{
							this->MoveCursorLeft();
							break;
						}
					case 77: //Right arrow
						{
							this->MoveCursorRight();
							break;
						}
					case 79: //Home
						{
							
							break;
						}
					case 80: //Down arrow
						{
							this->InputHistoryForward();
							break;
						}
					case 81: //Page down
						{
							this->GoOnePageDown();
							break;
						}
					case 82: //Insert
						{
							
							break;
						}
					case 83: //Delete
						{
							this->DeleteCharInInputBuffer();
							break;
						}
					case 133: //F11
						{
							
							break;
						}
					case 134: //F12
						{
							
							break;
						}
					}
					//printf("Control: 224 %d\n", ch);
					break;
				}
			case 8: //Backspace
				{
					this->BackspaceCharInInputBuffer();
					break;
				}
			case 13: //Enter
				{
					this->AcceptUserInput();
					break;
				}
			default:
				{
					this->AddCharToInputBuffer(ch);
				}
			}
		}
}

void Console::Print(const char *string, ...)
{
	va_list arglist; 
    va_start(arglist, string);
	int stringsize = _vscprintf(string, arglist);
	char *tempstring = (char *)calloc(stringsize + 1, sizeof(char));
	vsprintf(tempstring, string, arglist);
	va_end(arglist);
	this->WriteToOutputBuffer(tempstring);
	free(tempstring);
	this->PrintOutputPage();
	this->PrintInputLine();
}

unsigned short Console::GetNumberOfConsoleSymbols(void)
{
	return symbolbuffersize;
}

bool Console::IsConsoleSymbolExist(const char *name)
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

ConsoleSymbol *Console::GetConsoleSymbolByIndex(unsigned short index)
{
	if (index >= symbolbuffersize)
	{
		return NULL;
	}
	ConsoleSymbol *symbol = (ConsoleSymbol *)calloc(1, sizeof(ConsoleSymbol));
	memcpy(symbol, &symbolbuffer[index], sizeof(ConsoleSymbol));
	return symbol;
}

bool Console::AddConsoleSymbol(ConsoleSymbol *symbol)
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

bool Console::DeleteConsoleSymbol(const char *name)
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

char *Console::GetConsoleSymbolHelpString(const char *name)
{
	ConsoleSymbol *symbol = this->GetConsoleSymbol(name);
	if (symbol == NULL)
	{
		return NULL;
	}
	return this->GetHelpString(symbol);
}

char *Console::GetConsoleSymbolHelpStringByIndex(unsigned short index)
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

unsigned char Console::GetCmdArgs(void)
{
	return numargs;
}

bool Console::GetCmdArgString(char *&arg)
{
	if ((argpos == 0) || (commandbuffer == NULL))
	{
		return false;
	}
	arg = (char *)calloc(strlen(commandbuffer + argpos) + 1, sizeof(char));
	strcpy(arg, commandbuffer + argpos);
	return true;
}

bool Console::GetCmdArgType(const unsigned char argnum, ConVarType &type)
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

bool Console::GetCmdArg(const unsigned char argnum, char *&arg)
{
	if (argnum > numargs)
	{
		return false;
	}
	arg = (char *)calloc(strlen(commandargs[argnum]) + 1, sizeof(char));
	strcpy(arg, commandargs[argnum]);
	return true;
}

bool Console::GetCmdArg(const unsigned char argnum, int &arg)
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

bool Console::GetCmdArg(const unsigned char argnum, float &arg)
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

void Console::ClearScreen(void)
{
#ifdef WIN32
	system("cls");
#endif
}

void Console::ClearInputLine(void)
{
	unsigned int length = strlen(inputbuffer[inputbufferposition[0]]);
	this->SetCursorPosition(2, this->GetInputLineScreenPosition());
	for (unsigned int i = 0; i < length; i++)
	{
		printf(" ");
	}
}

void Console::SetCursorPosition(const unsigned char x, const unsigned char y)
{
#ifdef WIN32
	COORD coords = {x, y};
	SetConsoleCursorPosition(outputhandle, coords);
#endif
}

unsigned char Console::GetInputLineScreenPosition(void)
{
	if (outputbuffersize < pagesize)
	{
		return outputbuffersize + 1;
	}
	else
	{
		return pagesize + 1;
	}
}

void Console::PrintCaption(void)
{
	this->SetCursorPosition(0, 0);
#ifdef WIN32
#ifdef _UNICODE
	WriteConsole(outputhandle, L"FOUR-MP", sizeof("FOUR-MP"), NULL, NULL);
#else
	WriteConsole(outputhandle, "FOUR-MP", sizeof("FOUR-MP"), NULL, NULL);
#endif
	COORD coords = {0, 0};
	FillConsoleOutputAttribute(outputhandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY, 80, coords, NULL);
#else
	printf("Test\n");
#endif
}

void Console::PrintOutputPage(void)
{
	this->SetCursorPosition(0, 1);
	unsigned char length;
	for (unsigned char i = 0; ((i < pagesize) && (i < outputbuffersize)); i++)
	{
		printf("%s", outputbuffer[screenposition+i]);
		length = strlen(outputbuffer[screenposition+i]);
		if (length < 80)
		{
			do
			{
				printf(" ");
				length++;
			} while (length != 80);
		}
	}
}

void Console::PrintInputLine()
{
	this->SetCursorPosition(0, this->GetInputLineScreenPosition());
	printf("> %s", inputbuffer[inputbuffersize-1]);
}

void Console::UpdateScreen(void)
{
	this->PrintCaption();
	this->PrintOutputPage();
	this->PrintInputLine();
}

void Console::GoOnePageUp(void)
{
	if (screenposition == 0)
	{
		return;
	}
	else if (screenposition < pagesize)
	{
		screenposition = 0;
	}
	else
	{
		screenposition = screenposition - pagesize;
	}
	this->PrintOutputPage();
}

void Console::GoOnePageDown(void)
{
	if (screenposition == (outputbuffersize - pagesize))
	{
		return;
	}
	else if ((screenposition + pagesize) > (outputbuffersize - pagesize))
	{
		screenposition = outputbuffersize - pagesize;
	}
	else
	{
		screenposition = screenposition + pagesize;
	}
	this->PrintOutputPage();
}

void Console::InputHistoryBack(void)
{
	this->ClearInputLine();
	if (inputbufferposition[0] != 0)
	{
		inputbufferposition[0]--;
	}
	else
	{
		inputbufferposition[0] = inputbuffersize - 2;
	}
	this->FillInputBufferFromHistory();
	this->PrintInputLine();
}

void Console::InputHistoryForward(void)
{
	this->ClearInputLine();
	if (inputbufferposition[0] < (inputbuffersize - 2))
	{
		inputbufferposition[0]++;
	}
	else
	{
		inputbufferposition[0] = 0;
	}
	this->FillInputBufferFromHistory();
	this->PrintInputLine();
}

void Console::WriteToOutputBuffer(const char *string)
{
	unsigned int length = strlen(string);
	char *tempstring = (char *)calloc(81, sizeof(char));
	unsigned int i = 0;
	unsigned int j = 0;
	while (i < length)
	{
		switch (string[i])
		{
		case '\n':
			{
				tempstring[j] = '\0';
				this->AppendToOutputBuffer(tempstring);
				j = 0;
				break;
			}
		case '\t':
			{
				unsigned char pos = j % 8;
				unsigned char lastpos;
				if (pos < 7)
				{
					lastpos = 8;
				}
				else
				{
					lastpos = 16;
				}
				while (pos != lastpos)
				{
					tempstring[j] = ' ';
					j++;
					pos++;
					if (j == 80)
					{
						tempstring[80] = '\0';
						this->AppendToOutputBuffer(tempstring);
						j = 0;
					}
				}
				break;
			}
		default:
			{
				tempstring[j] = string[i];
				j++;
				break;
			}
		}
		if (j == 80)
		{
			tempstring[80] = '\0';
			this->AppendToOutputBuffer(tempstring);
			j = 0;
		}
		i++;
	}
	if (j != 0)
	{
		tempstring[j] = '\0';
		this->AppendToOutputBuffer(tempstring);
	}
	free(tempstring);
}

void Console::AppendToOutputBuffer(const char *string)
{
	if (outputbuffersize < maxoutputbuffersize)
		{
			this->ResizeArrayBuffer(outputbuffer, outputbuffersize + 1);
			outputbuffer[outputbuffersize] = (char *)calloc(81, sizeof(char));
			strcpy(outputbuffer[outputbuffersize], string);
			outputbuffersize++;
			if ((outputbuffersize - pagesize) > 0)
			{
				screenposition++;
			}
		}
	else
	{
		free(outputbuffer[0]);
		for (unsigned short i = 0; i < (maxoutputbuffersize - 1); i++)
		{
			outputbuffer[i] = outputbuffer[i+1];
		}
		outputbuffer[maxoutputbuffersize-1] = (char *)calloc(81, sizeof(char));
		strcpy(outputbuffer[maxoutputbuffersize-1], string);
	}
}

void Console::FillInputBufferFromHistory(void)
{
	unsigned int length = strlen(inputbuffer[inputbufferposition[0]]);
	if (strlen(inputbuffer[inputbuffersize-1]) != length)
	{
		this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], length + 1);
	}
	strcpy(inputbuffer[inputbuffersize-1], inputbuffer[inputbufferposition[0]]);
	inputbufferposition[1] = length;
}

void Console::AddCharToInputBuffer(const int ch)
{
	inputbufferposition[0] = inputbuffersize - 1;
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], buffersize + 2);
	char *tempstring = (char *)calloc((buffersize - inputbufferposition[1] + 1), sizeof(char));
	strncpy(tempstring, inputbuffer[inputbuffersize-1] + inputbufferposition[1], buffersize - inputbufferposition[1] + 1);
	inputbuffer[inputbuffersize-1][inputbufferposition[1]] = ch;
	strncpy(inputbuffer[inputbuffersize-1] + inputbufferposition[1] + 1, tempstring, buffersize - inputbufferposition[1] + 1);
	unsigned char y = this->GetInputLineScreenPosition();
	this->SetCursorPosition(inputbufferposition[1] + 2, y);
	inputbufferposition[1]++;
	printf("%c%s", ch, tempstring);
	free(tempstring);
	this->SetCursorPosition(inputbufferposition[1] + 2, y);
}

void Console::BackspaceCharInInputBuffer(void)
{
	if (inputbufferposition[1] == 0)
	{
		return;
	}
	inputbufferposition[0] = inputbuffersize - 1;
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	char *tempstring = (char *)calloc((buffersize - inputbufferposition[1] + 1), sizeof(char));
	strncpy(tempstring, inputbuffer[inputbuffersize-1] + inputbufferposition[1], buffersize - inputbufferposition[1] + 1);
	this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], buffersize);
	strncpy(inputbuffer[inputbuffersize-1] + inputbufferposition[1] - 1, tempstring, buffersize - inputbufferposition[1] + 1);
	inputbufferposition[1]--;
	unsigned char y = this->GetInputLineScreenPosition();
	this->SetCursorPosition(inputbufferposition[1] + 2, y);
	printf("%s ", tempstring);
	free(tempstring);
	this->SetCursorPosition(inputbufferposition[1] + 2, y);
}

void Console::MoveCursorLeft(void)
{
	if (inputbufferposition[1] == 0)
	{
		return;
	}
	inputbufferposition[1]--;
#ifdef WIN32
	this->SetCursorPosition(inputbufferposition[1] + 2, this->GetInputLineScreenPosition());
#else
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], buffersize + 2);
	inputbuffer[inputbuffersize-1][buffersize] = 8;
	inputbuffer[inputbuffersize-1][buffersize+1] = '\0';
	this->PrintInputLine();
#endif
}

void Console::MoveCursorRight(void)
{
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	if (inputbufferposition[1] == buffersize)
	{
		return;
	}
	inputbufferposition[1]++;
#ifdef WIN32
	this->SetCursorPosition(inputbufferposition[1] + 2, this->GetInputLineScreenPosition());
#else
	this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], buffersize);
	inputbuffer[inputbuffersize-1][buffersize-1] = '\0';
	this->PrintInputLine();
#endif
}

void Console::DeleteCharInInputBuffer(void)
{
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	if (inputbufferposition[1] == buffersize)
	{
		return;
	}
	inputbufferposition[0] = inputbuffersize - 1;
	char *tempstring = (char *)calloc((buffersize - inputbufferposition[1]), sizeof(char));
	strncpy(tempstring, inputbuffer[inputbuffersize-1] + inputbufferposition[1] + 1, buffersize - inputbufferposition[1]);
	this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], buffersize);
	strncpy(inputbuffer[inputbuffersize-1] + inputbufferposition[1], tempstring, buffersize - inputbufferposition[1]);
	unsigned char y = this->GetInputLineScreenPosition();
	this->SetCursorPosition(inputbufferposition[1] + 2, y);
	printf("%s ", tempstring);
	free(tempstring);
	this->SetCursorPosition(inputbufferposition[1] + 2, y);
}

void Console::AcceptUserInput(void)
{
	this->WriteToInputBuffer();
	inputbufferposition[0] = inputbuffersize - 1;
	inputbufferposition[1] = 0;
	size_t templength = strlen(inputbuffer[inputbuffersize-2]);
	char *tempstring = (char *)calloc(templength + 3, sizeof(char));
	sprintf(tempstring, "> %s", inputbuffer[inputbuffersize-2]);
	this->WriteToOutputBuffer(tempstring);
	this->InterpretLine(inputbuffer[inputbuffersize-2]);
	this->PrintInputLine();
}

void Console::WriteToInputBuffer(void)
{
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	unsigned int i = 0;
	do
	{
		if (inputbuffer[inputbuffersize-1][i] == 8)
		{
			this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], i + 1);
			inputbuffer[inputbuffersize-1][i] = '\0';
		}
		i++;
	} while (inputbuffer[inputbuffersize-1][i-1] != '\0');
	if (inputbuffersize < maxinputbuffersize)
		{
			this->ResizeArrayBuffer(inputbuffer, inputbuffersize + 1);
			inputbuffer[inputbuffersize] = (char *)calloc(1, sizeof(char));
			inputbuffersize++;
		}
	else
	{
		free(inputbuffer[0]);
		for (unsigned short i = 0; i < (maxinputbuffersize - 1); i++)
		{
			inputbuffer[i] = inputbuffer[i+1];
		}
		inputbuffer[maxinputbuffersize-1] = (char *)calloc(1, sizeof(char));
	}
}

bool Console::ResizeArrayBuffer(char **&buffer, const unsigned short size)
{
	char **tempbuffer = (char **)realloc(*&buffer, size * sizeof(char *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		printf("FATAL: Memory error");
		this->~Console();
		return false;
	}
	buffer = tempbuffer;
	return true;
}

bool Console::ResizeStringBuffer(char *&buffer, const unsigned int size)
{
	if (!(::ResizeStringBuffer(buffer, size)))
	{
		printf("FATAL: Memory error");
		this->~Console();
		return false;
	}
	return true;
}

bool Console::ResizeSymbolBuffer(ConsoleSymbol *&buffer, const unsigned short size)
{
	ConsoleSymbol *tempbuffer = (ConsoleSymbol *)realloc(buffer, size * sizeof(ConsoleSymbol));
	if ((tempbuffer == NULL) && (size != 0))
	{
		printf("FATAL: Memory error");
		this->~Console();
		return false;
	}
	buffer = tempbuffer;
	return true;
}

ConsoleSymbol *Console::GetConsoleSymbol(const char *name)
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

bool Console::GetConsoleSymbolIndex(const char *name, unsigned short &index)
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

char *Console::GetHelpString(const ConsoleSymbol *symbol)
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
				this->ResizeStringBuffer(helpstring, _scprintf("%s = \"%f\"", helpstring, value) + 1);
				sprintf(helpstring, "%s = \"%f\"", helpstring, value);
				break;
			}
		case ConVarTypeInt:
			{
				int value;
				symbol->ptr.convar->GetValue(value);
				this->ResizeStringBuffer(helpstring, _scprintf("%s = \"%d\"", helpstring, value) + 1);
				sprintf(helpstring, "%s = \"%d\"", helpstring, value);
				break;
			}
		case ConVarTypeString:
			{
				char *value;
				symbol->ptr.convar->GetValue(value);
				this->ResizeStringBuffer(helpstring, _scprintf("%s = \"%s\"", helpstring, value) + 1);
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
				this->ResizeStringBuffer(helpstring, _scprintf("%s ( def. \"%f\" )", helpstring, value) + 1);
				sprintf(helpstring, "%s ( def. \"%f\" )", helpstring, value);
				if (symbol->ptr.convar->GetBoundFloat(ConVarBoundLower, value))
				{
					this->ResizeStringBuffer(helpstring, _scprintf("%s min. %f", helpstring, value) + 1);
					sprintf(helpstring, "%s min. %f", helpstring, value);
				}
				if (symbol->ptr.convar->GetBoundFloat(ConVarBoundUpper, value))
				{
					this->ResizeStringBuffer(helpstring, _scprintf("%s max. %f", helpstring, value) + 1);
					sprintf(helpstring, "%s max. %f", helpstring, value);
				}
				break;
			}
		case ConVarTypeInt:
			{
				int value;
				symbol->ptr.convar->GetDefaultValue(value);
				this->ResizeStringBuffer(helpstring, _scprintf("%s ( def. \"%d\" )", helpstring, value) + 1);
				sprintf(helpstring, "%s ( def. \"%d\" )", helpstring, value);
				if (symbol->ptr.convar->GetBoundInt(ConVarBoundLower, value))
				{
					this->ResizeStringBuffer(helpstring, _scprintf("%s min. %d", helpstring, value) + 1);
					sprintf(helpstring, "%s min. %d", helpstring, value);
				}
				if (symbol->ptr.convar->GetBoundInt(ConVarBoundUpper, value))
				{
					this->ResizeStringBuffer(helpstring, _scprintf("%s max. %d", helpstring, value) + 1);
					sprintf(helpstring, "%s max. %d", helpstring, value);
				}
				break;
			}
		case ConVarTypeString:
			{
				char *value;
				symbol->ptr.convar->GetDefaultValue(value);
				this->ResizeStringBuffer(helpstring, _scprintf("%s  ( def. \"%s\" )", helpstring, value) + 1);
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
			this->ResizeStringBuffer(helpstring, _scprintf("%s\n - %s", helpstring, desc) + 1);
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
			this->ResizeStringBuffer(helpstring, _scprintf("%s\n - %s", helpstring, desc) + 1);
			sprintf(helpstring, "%s\n - %s", helpstring, desc);
		}
		free(desc);
	}
	return helpstring;
}

void Console::InterpretLine(const char *string)
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

char *Console::GetCommand(const char *string, const unsigned int startindex)
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

void Console::InterpretCommand(void)
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
		this->Print("Unknown command \"%s\"", symbolname);
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
			this->ResizeArrayBuffer(commandargs, j + 1);
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
			this->ResizeArrayBuffer(commandargs, j + 1);
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
				this->Print(this->GetHelpString(symbol));
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
