#pragma once

#include "ConVar.h"
#include "ConCmd.h"

class ConsoleCore
{
public:
	ConsoleCore(void);
	~ConsoleCore(void);
	void SetOutputFunction(void *function); //Sets which function console will use to output text
	void Output(const char *string, ...); //Outputs text into outside function
	unsigned short GetNumberOfConsoleSymbols(void); //Returns total number of console symbols
	bool IsConsoleSymbolExist(const char *name); //Checks if symbol exists
	ConVar *AddConVar(const char *name, const float defaultvalue, const char *description = "", const int flags = 0, const bool hasMin = false, const float min = 0.0, const bool hasMax = false, const float max = 0.0); //Adds console variable into symbol buffer and returns pointer to it
	ConVar *AddConVar(const char *name, const int defaultvalue, const char *description = "", const int flags = 0, const bool hasMin = false, const int min = 0, const bool hasMax = false, const int max = 0); //Adds console variable into symbol buffer and returns pointer to it
	ConVar *AddConVar(const char *name, const char *defaultvalue, const char *description = "", const int flags = 0); //Adds console variable into symbol buffer and returns pointer to it
	ConCmd *AddConCmd(const char *name, void *callback, const char *description = "", const int flags = 0); //Adds console command into symbol buffer and returns pointer to it
	char *GetConsoleSymbolHelpString(const char *name); //Returns help string of given symbol
	char *GetConsoleSymbolHelpStringByIndex(unsigned short index); //Returns help string of given symbol index
	unsigned char GetCmdArgs(void); //Returns the number of arguments from the current console command
	bool GetCmdArgString(char *&arg); //Retrieves the entire command argument string in one lump from the current console command
	bool GetCmdArgType(const unsigned char argnum, ConVarType &type); //Retrieves a command argument type given its index, from the current console command
	bool GetCmdArg(const unsigned char argnum, char *&arg); //Retrieves a command argument given its index, from the current console command
	bool GetCmdArg(const unsigned char argnum, int &arg);	//Retrieves a command argument given its index, from the current console command
	bool GetCmdArg(const unsigned char argnum, float &arg); //Retrieves a command argument given its index, from the current console command
	void InterpretLine(const char *string); //Interprets line
private:
	void *outputfunction; //Holds pointer to the function which is used to output text
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
		ConsoleSymbolPtr *ptr;
		unsigned char numcmds;
	};
	ConsoleSymbol *symbolbuffer; //Holds all registered console symbols
	unsigned short symbolbuffersize; //Holds current size of the symbol buffer
	unsigned short maxsymbolbuffersize; // Holds maximum size of the symbol buffer
	unsigned char maxcmdspersymbol;
	char *commandbuffer; //Holds current command string
	char **commandargs; //Holds arguments of current command
	unsigned int argpos; //Holds starting index of first argument in the current command
	unsigned char numargs; //Holds number of arguments in the current command
	bool DeleteConsoleSymbolByIndex(const unsigned short index); //Deletes console symbol from symbol buffer by it's index
	void DeleteConVar(const char *name); //Deletes console variable from symbol buffer
	void DeleteConCmd(const char *name, ConCmd *ptr); //Deletes console command from symbol buffer
	ConsoleSymbol *GetConsoleSymbol(const char *name); //Returns console index by name
	ConsoleSymbol *GetConsoleSymbolByIndex(unsigned short index); //Returns console symbol by it's index
	bool GetConsoleSymbolIndex(const char *name, unsigned short &index); //Returns symbol index by it's name
	char *GetHelpString(const ConsoleSymbol *symbol); //Returns help string of given symbol
	bool ResizeSymbolBuffer(ConsoleSymbol *&buffer, const unsigned short size); //Wrapper for realloc (don't touch)
	bool ResizeSymbolPtrBuffer(ConsoleSymbolPtr *&buffer, const unsigned char size); //Wrapper for realloc (don't touch)
	char *GetCommand(const char *string, const unsigned int startindex); //Gets command string from multicommand string starting from startindex
	void InterpretCommand(void); //Interpret prepared command
	friend class ConVar;
	friend class ConCmd;
	friend void ConCmdCvarlist(unsigned char numargs);
};
