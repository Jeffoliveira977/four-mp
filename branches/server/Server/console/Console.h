#pragma once

#include "ConVar.h"
#include "ConCmd.h"

#ifdef WIN32
#include <windows.h>
#endif

enum ConsoleSymbolType
{
	ConsoleSymbolTypeConVar,
	ConsoleSymbolTypeConCmd
};

union ConsoleSymbolPtr
{
	ConVar *convar;
	ConCmd *concmd;
};

struct ConsoleSymbol
{
	char *name;
	ConsoleSymbolType type;
	ConsoleSymbolPtr ptr;
};

class Console
{
public:
	Console(void);
	~Console(void);
	void CheckUserInput(void); //Handles keyboard input
	void Print(const char *string, ...); //Prints string to the output page
	unsigned short GetNumberOfConsoleSymbols(void);
	bool IsConsoleSymbolExist(const char *name);
	ConsoleSymbol *GetConsoleSymbolByIndex(unsigned short index);
	bool AddConsoleSymbol(ConsoleSymbol *symbol);
	bool DeleteConsoleSymbol(const char *name);
	char *GetConsoleSymbolHelpString(const char *name);
	char *GetConsoleSymbolHelpStringByIndex(unsigned short index);
	unsigned char GetCmdArgs(void); //Returns the number of arguments from the current console command
	bool GetCmdArgString(char *&arg); //Retrieves the entire command argument string in one lump from the current console command
	bool GetCmdArgType(const unsigned char argnum, ConVarType &type);
	bool GetCmdArg(const unsigned char argnum, char *&arg); //Retrieves a command argument given its index, from the current console command
	bool GetCmdArg(const unsigned char argnum, int &arg);	//Retrieves a command argument given its index, from the current console command
	bool GetCmdArg(const unsigned char argnum, float &arg); //Retrieves a command argument given its index, from the current console command
	void InterpretLine(const char *string); //Interprets line
private:
	char **outputbuffer;
	char **inputbuffer;
#ifdef WIN32
	HANDLE outputhandle;
#endif
	unsigned short outputbuffersize; //Holds current size of the output buffer (in lines)
	unsigned short maxoutputbuffersize; // Holds maximum size of the output buffer (in lines)
	unsigned char inputbuffersize; //Holds curent size of the input buffer (in lines)
	unsigned char maxinputbuffersize; // Holds maximum size of the input buffer (in lines)
	ConsoleSymbol *symbolbuffer;
	unsigned short symbolbuffersize; //Holds current size of the symbol buffer
	unsigned short maxsymbolbuffersize; // Holds maximum size of the symbol buffer
	char *commandbuffer; //Holds current command string
	char **commandargs; //Holds arguments of current command
	unsigned int argpos; //Holds starting index of first argument in the current command
	unsigned char numargs; //Holds number of arguments in the current command
	unsigned char inputbufferposition[2]; //Holds current cursor position in the input buffer ([0] - Line index, [1] - Character index)
	unsigned char pagesize; //How many lines will be shown on one page (excluding caption and input line)
	unsigned short screenposition; //Holds index of the first line in the output buffer that will be shown on the current page
	void ClearScreen(void); //Clears the screen
	void ClearInputLine(void); //Clears input line
	void SetCursorPosition(const unsigned char x, const unsigned char y); //Sets cursor position on the console screen
	unsigned char GetInputLineScreenPosition(void); //Gets y coordinate of the input line on the console screen
	void PrintCaption(void); //Prints caption
	void PrintOutputPage(void); //Prints output page
	void PrintInputLine(void); //Prints input line
	void UpdateScreen(void); //Performs full update of screen
	void GoOnePageUp(void); //Goes up one page
	void GoOnePageDown(void); //Goes down one page
	void InputHistoryBack(void); //Goes one entry back in the input history
	void InputHistoryForward(void); //Goes one entry forward in the input history
	void WriteToOutputBuffer(const char *string); //Appends string to the output buffer
	void AppendToOutputBuffer(const char *string); //Writes prepared string to the output buffer (don't touch)
	void FillInputBufferFromHistory(void); //Replaces current input with one from history
	void AddCharToInputBuffer(const int ch); //Adds character to the current position in the input buffer
	void BackspaceCharInInputBuffer(void); //Backspaces from the current position in the input buffer
	void MoveCursorLeft(void); //Moves cursor left in the input buffer
	void MoveCursorRight(void); //Moves cursor right in the input buffer
	void DeleteCharInInputBuffer(void); //Deletes character from the current position in the input buffer
	void AcceptUserInput(void); //Accepts inputted line
	void WriteToInputBuffer(void); //Appends current string to the input buffer
	bool ResizeArrayBuffer(char **&buffer, const unsigned short size); //Wrapper for realloc (don't touch)
	bool ResizeStringBuffer(char *&buffer, const unsigned int size); //Wrapper for realloc (don't touch)
	bool ResizeSymbolBuffer(ConsoleSymbol *&buffer, const unsigned short size); //Wrapper for realloc (don't touch)
	ConsoleSymbol *GetConsoleSymbol(const char *name);
	bool GetConsoleSymbolIndex(const char *name, unsigned short &index);
	char *GetHelpString(const ConsoleSymbol *symbol);
	char *GetCommand(const char *string, const unsigned int startindex); //Gets command string from multicommand string starting from startindex
	void InterpretCommand(void); //Interpret prepared command
};
