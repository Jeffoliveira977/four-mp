#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>

#include "Console.h"

Console::Console(void)
{
	maxoutputbuffersize = 1024;
	maxinputbuffersize = 255;
	pagesize = 26;
	outputbuffer = (char **)calloc(1, sizeof(char *));
	outputbuffersize = 0;
	inputbuffer = (char **)calloc(1, sizeof(char *));
	inputbuffer[0] = (char *)calloc(1, sizeof(char));
	inputbuffersize = 1;
	inputbufferposition[0] = 0;
	inputbufferposition[1] = 0;
	screenposition = 0;
	this->UpdateScreen();
}

Console::~Console(void)
{
	short i;
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
	char *tempstring = (char *)calloc(stringsize+1, sizeof(char));
	vsprintf(tempstring, string, arglist);
	va_end(arglist);
	this->WriteToOutputBuffer(tempstring);
	this->UpdateScreen();
	//FMP specific
	FILE *f = fopen("server.log", "a");
	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(f, "[%02d.%02d %02d:%02d:%02d] %s\n", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond, tempstring);
	fclose(f);
	//END FMP specific
	free(tempstring);
}

void Console::Debug(const char *string, ...)
{
	va_list arglist; 
    va_start(arglist, string);
	int stringsize = _vscprintf(string, arglist);
	char *tempstring = (char *)calloc(stringsize+1, sizeof(char));
	vsprintf(tempstring, string, arglist);
	va_end(arglist);
	this->WriteToOutputBuffer(tempstring);
	this->UpdateScreen();
	//FMP specific
	FILE *f = fopen("server.log", "a");
	SYSTEMTIME time;
	GetSystemTime(&time);
	fprintf(f, "<%02d.%02d %02d:%02d:%02d> %s\n", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond, tempstring);
	fclose(f);
	//END FMP specific
	free(tempstring);
}

void Console::ClearScreen(void)
{
	system("cls");
}

void Console::PrintCaption(void)
{
	printf("Test\n");
}

void Console::PrintOutputPage(void)
{
	for (char i = 0; ((i < pagesize) && (i < outputbuffersize)); i++)
	{
		if (strlen(outputbuffer[screenposition+i]) < 80)
		{
			printf("%s\n", outputbuffer[screenposition+i]);
		}
		else
		{
			printf("%s", outputbuffer[screenposition+i]);
		}
	}
}

void Console::PrintInputLine()
{
	printf("> %s", inputbuffer[inputbuffersize-1]);
}

void Console::UpdateScreen(void)
{
	this->ClearScreen();
	this->PrintCaption();
	this->PrintOutputPage();
	this->PrintInputLine();
}

void Console::GoOnePageUp(void)
{
	if ((screenposition - pagesize) < 0)
	{
		screenposition = 0;
	}
	else
	{
		screenposition = screenposition - pagesize;
	}
	this->UpdateScreen();
}

void Console::GoOnePageDown(void)
{
	if ((screenposition + pagesize) > (outputbuffersize - pagesize))
	{
		screenposition = outputbuffersize - pagesize;
	}
	else
	{
		screenposition = screenposition + pagesize;
	}
	this->UpdateScreen();
}

void Console::InputHistoryBack(void)
{
	if (inputbufferposition[0] != 0)
	{
		inputbufferposition[0]--;
	}
	else
	{
		inputbufferposition[0] = inputbuffersize - 2;
	}
	this->FillInputBufferFromHistory();
	this->UpdateScreen();
}

void Console::InputHistoryForward(void)
{
	if (inputbufferposition[0] < (inputbuffersize - 2))
	{
		inputbufferposition[0]++;
	}
	else
	{
		inputbufferposition[0] = 0;
	}
	this->FillInputBufferFromHistory();
	this->UpdateScreen();
}

void Console::WriteToOutputBuffer(const char *string)
{
	unsigned int length = strlen(string);
	unsigned int numstrings = length / 80;
	unsigned char lastlength = length % 80;
	char *tempstring = (char *)calloc(81, sizeof(char));
	for (unsigned int i = 0; i < numstrings; i++)
	{
		strncpy(tempstring, string + i * 80, 80);
		this->AppendToOutputBuffer(tempstring);
	}
	if (lastlength != 0)
	{
		strncpy(tempstring, string + numstrings * 80, lastlength);
		tempstring[lastlength] = '\0';
		this->AppendToOutputBuffer(tempstring);
	}
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
	this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], length + 1);
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
	inputbufferposition[1]++;
	free(tempstring);
	this->UpdateScreen();
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
	free(tempstring);
	this->UpdateScreen();
}

void Console::MoveCursorLeft(void)
{
	if (inputbufferposition[1] == 0)
	{
		return;
	}
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], buffersize + 2);
	inputbuffer[inputbuffersize-1][buffersize] = 8;
	inputbuffer[inputbuffersize-1][buffersize+1] = '\0';
	inputbufferposition[1]--;
	this->UpdateScreen();
}

void Console::MoveCursorRight(void)
{
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	if (inputbufferposition[1] == buffersize)
	{
		return;
	}
	this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], buffersize);
	inputbuffer[inputbuffersize-1][buffersize-1] = '\0';
	inputbufferposition[1]++;
	this->UpdateScreen();
}

void Console::DeleteCharInInputBuffer(void)
{
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	if (inputbufferposition[1] == (buffersize + 1))
	{
		return;
	}
	inputbufferposition[0] = inputbuffersize - 1;
	char *tempstring = (char *)calloc((buffersize - inputbufferposition[1]), sizeof(char));
	strncpy(tempstring, inputbuffer[inputbuffersize-1] + inputbufferposition[1] + 1, buffersize - inputbufferposition[1]);
	this->ResizeStringBuffer(inputbuffer[inputbuffersize-1], buffersize);
	strncpy(inputbuffer[inputbuffersize-1] + inputbufferposition[1], tempstring, buffersize - inputbufferposition[1]);
	free(tempstring);
	this->UpdateScreen();
}

void Console::AcceptUserInput(void)
{
	inputbufferposition[1] = 0;
	this->WriteToInputBuffer();
	size_t templength = strlen(inputbuffer[inputbuffersize-2]);
	char *tempstring = (char *)calloc(templength + 3, sizeof(char));
	sprintf(tempstring, "> %s", inputbuffer[inputbuffersize-2]);
	this->WriteToOutputBuffer(tempstring);
	this->InterpretLine(inputbuffer[inputbuffersize-2]);
	this->UpdateScreen();
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

void Console::ResizeArrayBuffer(char **&buffer, const unsigned short size)
{
	char **tempbuffer = (char **)realloc(*&buffer, size * sizeof(char *));
	if (tempbuffer == NULL)
	{
		printf("FATAL: Memory error");
		this->~Console();
	}
	buffer = tempbuffer;
}

void Console::ResizeStringBuffer(char *&buffer, const unsigned int size)
{
	char *tempbuffer = (char *)realloc(buffer, size * sizeof(char));
	if (tempbuffer == NULL)
	{
		printf("FATAL: Memory error");
		this->~Console();
	}
	buffer = tempbuffer;
}

void Console::InterpretLine(const char *string)
{

}
