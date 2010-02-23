#pragma once

#include "ConVar.h"
#include "ConCmd.h"

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

class ConsoleCore
{
public:
	ConsoleCore(void);
	~ConsoleCore(void);
	void SetOutputFunction(void *function);
	void Output(const char *string, ...);
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
	void *outputfunction;
	ConsoleSymbol *symbolbuffer;
	unsigned short symbolbuffersize; //Holds current size of the symbol buffer
	unsigned short maxsymbolbuffersize; // Holds maximum size of the symbol buffer
	char *commandbuffer; //Holds current command string
	char **commandargs; //Holds arguments of current command
	unsigned int argpos; //Holds starting index of first argument in the current command
	unsigned char numargs; //Holds number of arguments in the current command
	bool ResizeSymbolBuffer(ConsoleSymbol *&buffer, const unsigned short size); //Wrapper for realloc (don't touch)
	ConsoleSymbol *GetConsoleSymbol(const char *name);
	bool GetConsoleSymbolIndex(const char *name, unsigned short &index);
	char *GetHelpString(const ConsoleSymbol *symbol);
	char *GetCommand(const char *string, const unsigned int startindex); //Gets command string from multicommand string starting from startindex
	void InterpretCommand(void); //Interpret prepared command
};
