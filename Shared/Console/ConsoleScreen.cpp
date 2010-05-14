/// \file
/// \brief Source file that contains implementation of the ConsoleScreen class.
/// \details See class description.
/// \author FaTony

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdarg.h>

#include "ConsoleScreen.h"
#include "common.h"

ConsoleScreen::ConsoleScreen(ConsoleCore *core)
{
	concore = core;
	maxoutputbuffersize = 1024;
	maxinputbuffersize = 255;
	pagesize = 23;
#ifdef WIN32
	outputhandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	caption = (char *)calloc(2, sizeof(char));
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

ConsoleScreen::~ConsoleScreen(void)
{
	free(caption);
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
}

void ConsoleScreen::SetCaption(const char *string)
{
	ResizeBuffer<char *>(caption, strlen(string) + 1);
	strcpy(caption, string);
	this->PrintCaption();
	this->SetFinalCursorPosition();
}

void ConsoleScreen::CheckUserInput(void)
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
					case 79: //End
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

void ConsoleScreen::Print(const char *string, ...)
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

void ConsoleScreen::ClearScreen(void)
{
#ifdef WIN32
	system("cls");
#endif
}

void ConsoleScreen::ClearInputLine(void)
{
	unsigned int length = strlen(inputbuffer[inputbufferposition[0]]);
	this->SetCursorPosition(2, this->GetInputLineScreenPosition());
	for (unsigned int i = 0; i < length; i++)
	{
		printf(" ");
	}
}

void ConsoleScreen::SetCursorPosition(const unsigned char x, const unsigned char y)
{
#ifdef WIN32
	COORD coords = {x, y};
	SetConsoleCursorPosition(outputhandle, coords);
#endif
}

unsigned char ConsoleScreen::GetInputLineScreenPosition(void)
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

void ConsoleScreen::SetFinalCursorPosition(void)
{
	this->SetCursorPosition((strlen(inputbuffer[inputbuffersize-1]) + 2) % 80, this->GetInputLineScreenPosition());
}

void ConsoleScreen::PrintCaption(void)
{
	this->SetCursorPosition(0, 0);
#ifdef WIN32
	unsigned int length = strlen(caption);
	DWORD temp;
#ifdef _UNICODE
	wchar_t *tempstring = (wchar_t *)calloc(length + 1, sizeof(wchar_t));
	mbstowcs(tempstring, caption, length + 1);
	WriteConsole(outputhandle, tempstring, length, &temp, NULL);
	free(tempstring);
#else
	WriteConsole(outputhandle, caption, length, &temp, NULL);
#endif
	COORD coords = {0, 0};
	FillConsoleOutputAttribute(outputhandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY, 80, coords, &temp);
#else
	printf(caption);
#endif
}

void ConsoleScreen::PrintOutputPage(void)
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

void ConsoleScreen::PrintInputLine()
{
	this->SetCursorPosition(0, this->GetInputLineScreenPosition());
	printf("> %s", inputbuffer[inputbuffersize-1]);
}

void ConsoleScreen::UpdateScreen(void)
{
	this->PrintCaption();
	this->PrintOutputPage();
	this->PrintInputLine();
}

void ConsoleScreen::GoOnePageUp(void)
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
	this->SetFinalCursorPosition();
}

void ConsoleScreen::GoOnePageDown(void)
{
	if (screenposition >= (outputbuffersize - pagesize))
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
	this->SetFinalCursorPosition();
}

void ConsoleScreen::InputHistoryBack(void)
{
	this->ClearInputLine();
	if (inputbufferposition[0] != 0)
	{
		inputbufferposition[0]--;
	}
	else
	{
		if (inputbuffersize != 1)
		{
			inputbufferposition[0] = inputbuffersize - 2;
		}
		else
		{
			inputbufferposition[0] = 0;
		}
	}
	this->FillInputBufferFromHistory();
	this->PrintInputLine();
}

void ConsoleScreen::InputHistoryForward(void)
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

void ConsoleScreen::WriteToOutputBuffer(const char *string)
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

void ConsoleScreen::AppendToOutputBuffer(const char *string)
{
	if (strlen(string) > 80)
	{
		this->WriteToOutputBuffer(string);
		return;
	}
	if (outputbuffersize < maxoutputbuffersize)
		{
			ResizeBuffer<char **>(outputbuffer, outputbuffersize + 1);
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

void ConsoleScreen::FillInputBufferFromHistory(void)
{
	unsigned int length = strlen(inputbuffer[inputbufferposition[0]]);
	if (strlen(inputbuffer[inputbuffersize-1]) != length)
	{
		ResizeBuffer<char *>(inputbuffer[inputbuffersize-1], length + 1);
	}
	strcpy(inputbuffer[inputbuffersize-1], inputbuffer[inputbufferposition[0]]);
	inputbufferposition[1] = length;
}

void ConsoleScreen::AddCharToInputBuffer(const int ch)
{
	inputbufferposition[0] = inputbuffersize - 1;
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	ResizeBuffer<char *>(inputbuffer[inputbuffersize-1], buffersize + 2);
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

void ConsoleScreen::BackspaceCharInInputBuffer(void)
{
	if (inputbufferposition[1] == 0)
	{
		return;
	}
	inputbufferposition[0] = inputbuffersize - 1;
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	char *tempstring = (char *)calloc((buffersize - inputbufferposition[1] + 1), sizeof(char));
	strncpy(tempstring, inputbuffer[inputbuffersize-1] + inputbufferposition[1], buffersize - inputbufferposition[1] + 1);
	ResizeBuffer<char *>(inputbuffer[inputbuffersize-1], buffersize);
	strncpy(inputbuffer[inputbuffersize-1] + inputbufferposition[1] - 1, tempstring, buffersize - inputbufferposition[1] + 1);
	inputbufferposition[1]--;
	unsigned char y = this->GetInputLineScreenPosition();
	this->SetCursorPosition(inputbufferposition[1] + 2, y);
	printf("%s ", tempstring);
	free(tempstring);
	this->SetCursorPosition(inputbufferposition[1] + 2, y);
}

void ConsoleScreen::MoveCursorLeft(void)
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
	ResizeBuffer<char *, char, unsigned int>(inputbuffer[inputbuffersize-1], buffersize + 2);
	inputbuffer[inputbuffersize-1][buffersize] = 8;
	inputbuffer[inputbuffersize-1][buffersize+1] = '\0';
	this->PrintInputLine();
#endif
}

void ConsoleScreen::MoveCursorRight(void)
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
	ResizeBuffer<char *, char, unsigned int>(inputbuffer[inputbuffersize-1], buffersize);
	inputbuffer[inputbuffersize-1][buffersize-1] = '\0';
	this->PrintInputLine();
#endif
}

void ConsoleScreen::DeleteCharInInputBuffer(void)
{
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	if (inputbufferposition[1] == buffersize)
	{
		return;
	}
	inputbufferposition[0] = inputbuffersize - 1;
	char *tempstring = (char *)calloc((buffersize - inputbufferposition[1]), sizeof(char));
	strncpy(tempstring, inputbuffer[inputbuffersize-1] + inputbufferposition[1] + 1, buffersize - inputbufferposition[1]);
	ResizeBuffer<char *>(inputbuffer[inputbuffersize-1], buffersize);
	strncpy(inputbuffer[inputbuffersize-1] + inputbufferposition[1], tempstring, buffersize - inputbufferposition[1]);
	unsigned char y = this->GetInputLineScreenPosition();
	this->SetCursorPosition(inputbufferposition[1] + 2, y);
	printf("%s ", tempstring);
	free(tempstring);
	this->SetCursorPosition(inputbufferposition[1] + 2, y);
}

void ConsoleScreen::AcceptUserInput(void)
{
	if (outputbuffersize >= pagesize)
	{
		this->ClearInputLine();
	}
	this->WriteToInputBuffer();
	inputbufferposition[0] = inputbuffersize - 1;
	inputbufferposition[1] = 0;
	size_t templength = strlen(inputbuffer[inputbuffersize-2]);
	char *tempstring = (char *)calloc(templength + 3, sizeof(char));
	sprintf(tempstring, "> %s", inputbuffer[inputbuffersize-2]);
	this->WriteToOutputBuffer(tempstring);
	concore->InterpretLine(inputbuffer[inputbuffersize-2]);
	this->PrintInputLine();
}

void ConsoleScreen::WriteToInputBuffer(void)
{
	unsigned int buffersize = strlen(inputbuffer[inputbuffersize-1]);
	unsigned int i = 0;
	do
	{
		if (inputbuffer[inputbuffersize-1][i] == 8)
		{
			ResizeBuffer<char *>(inputbuffer[inputbuffersize-1], i + 1);
			inputbuffer[inputbuffersize-1][i] = '\0';
		}
		i++;
	} while (inputbuffer[inputbuffersize-1][i-1] != '\0');
	if (inputbuffersize < maxinputbuffersize)
		{
			ResizeBuffer<char **>(inputbuffer, inputbuffersize + 1);
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

