/// \file
/// \brief Main header that describes the ConsoleCore class.
/// \details See class description.
/// \author FaTony

#pragma once

#include "ConVar.h"
#include "ConCmd.h"

/// \brief The main class that handles all logic behind the console.
/// \details Console Core is a system responsible for interpreting text commands. It takes
/// string of text as input and translates it into function calls. It also keeps track of
/// all associated variables and commands and dynamically loads and unloads them. It
/// recieves input as char * and use developer-defined function to output text so any
/// developer can easily code a wrapper around it.
/// \author FaTony

class ConsoleCore
{
public:

	/// \brief Inits the core.
	ConsoleCore(void);

	/// \brief Unloads everything.
	~ConsoleCore(void);

	/// \brief Sets which function the console will use to output text.
	/// \param[in] function Pointer to the function.
	/// \note Function should be void func(const char *string); or void func(const char *string, ...);
	/// \return No return.
	void SetOutputFunction(void *function);

	/// \brief Outputs text into the outside function.
	/// \param[in] string Format-control string.
	/// \param[in] ... Optional arguments.
	/// \return No return.
	void Output(const char *string, ...);

	/// \brief Returns total number of console symbols.
	/// \return Total number of console symbols.
	unsigned short GetNumberOfConsoleSymbols(void);

	/// \brief Checks if console symbol exists
	/// \param[in] name Name of the console symbol.
	/// \return True if the symbol exists, false otherwise.
	bool IsConsoleSymbolExist(const char *name); 

	/// \brief Adds console variable into symbol buffer and returns pointer to it.
	/// \param[in] name Name of the console variable.
	/// \param[in] defaultvalue Default value of the console variable.
	/// \param[in] description Optional description of the console variable.
	/// \param[in] flags Optional bitstring of flags determining how the console variable should be handled. See FCVAR_* constants for more details.
	/// \param[in] hasMin Optional boolean that determines if the console variable has a minimum value.
	/// \param[in] min Minimum value that the console variable can have if hasMin is true.
	/// \param[in] hasMax Optional boolean that determines if the console variable has a maximum value.
	/// \param[in] max Maximum value that the console variable can have if hasMax is true.
	/// \return Pointer to the console variable on success, NULL otherwise.
	ConVar *AddConVar(const char *name, const float defaultvalue, const char *description = "", const int flags = 0, const bool hasMin = false, const float min = 0.0, const bool hasMax = false, const float max = 0.0); 

	/// \brief Adds console variable into symbol buffer and returns pointer to it.
	/// \param[in] name Name of the console variable.
	/// \param[in] defaultvalue Default value of the console variable.
	/// \param[in] description Optional description of the console variable.
	/// \param[in] flags Optional bitstring of flags determining how the console variable should be handled. See FCVAR_* constants for more details.
	/// \param[in] hasMin Optional boolean that determines if the console variable has a minimum value.
	/// \param[in] min Minimum value that the console variable can have if hasMin is true.
	/// \param[in] hasMax Optional boolean that determines if the console variable has a maximum value.
	/// \param[in] max Maximum value that the console variable can have if hasMax is true.
	/// \return Pointer to the console variable on success, NULL otherwise.
	ConVar *AddConVar(const char *name, const int defaultvalue, const char *description = "", const int flags = 0, const bool hasMin = false, const int min = 0, const bool hasMax = false, const int max = 0);

	/// \brief Adds console variable into symbol buffer and returns pointer to it.
	/// \param[in] name Name of the console variable.
	/// \param[in] defaultvalue Default value of the console variable.
	/// \param[in] description Optional description of the console variable.
	/// \param[in] flags Optional bitstring of flags determining how the console variable should be handled. See FCVAR_* constants for more details.
	/// \return Pointer to the console variable on success, NULL otherwise.
	ConVar *AddConVar(const char *name, const char *defaultvalue, const char *description = "", const int flags = 0);

	/// \brief Adds console command into symbol buffer and returns pointer to it.
	/// \param[in] name Name of the console command.
	/// \param[in] callback A function to use as a callback for when the command is invoked.
	/// \param[in] description Optional description of the console command.
	/// \param[in] flags Optional bitstring of flags determining how the console command should be handled. See FCVAR_* constants for more details.
	/// \note Callback should be void func(unsigned char numargs);
	/// \return Pointer to the console variable on success, NULL otherwise.
	ConCmd *AddConCmd(const char *name, void *callback, const char *description = "", const int flags = 0);

	/// \brief Returns help string of given symbol.
	/// \param[in] name Name of the console symbol to search for.
	/// \return Help string on success, NULL otherwise.
	char *GetConsoleSymbolHelpString(const char *name);

	/// \brief Returns help string of given symbol index.
	/// \param[in] index Index of the console symbol.
	/// \return Help string on success, NULL otherwise.
	char *GetConsoleSymbolHelpStringByIndex(unsigned short index);

	/// \brief Returns the number of arguments from the current console command.
	/// \return Number of arguments from the current console command.
	unsigned char GetCmdArgs(void);

	/// \brief Retrieves the entire command argument string in one lump from the current console command.
	/// \param[out] arg Buffer to use for storing the string.
	/// \return True on success, false otherwise.
	bool GetCmdArgString(char *&arg);

	/// \brief Retrieves a command argument type given its index, from the current console command.
	/// \param[in] argnum Argument number to retrieve.
	/// \param[out] type Buffer to use for storing the type.
	/// \return True on success, false otherwise.
	bool GetCmdArgType(const unsigned char argnum, ConVarType &type);

	/// \brief Retrieves a command argument given its index, from the current console command.
	/// \param[in] argnum Argument number to retrieve.
	/// \param[out] arg Buffer to use for storing the value.
	/// \return True on success, false otherwise.
	bool GetCmdArg(const unsigned char argnum, char *&arg);

	/// \brief Retrieves a command argument given its index, from the current console command.
	/// \param[in] argnum Argument number to retrieve.
	/// \param[out] arg Buffer to use for storing the value.
	/// \return True on success, false otherwise.
	bool GetCmdArg(const unsigned char argnum, int &arg);

	/// \brief Retrieves a command argument given its index, from the current console command.
	/// \param[in] argnum Argument number to retrieve.
	/// \param[out] arg Buffer to use for storing the value.
	/// \return True on success, false otherwise.
	bool GetCmdArg(const unsigned char argnum, float &arg);

	/// \brief Interprets line.
	/// \param[in] string Line of text to interpret.
	/// \note The input string can contain several commands separated by ;.
	/// \return No return.
	void InterpretLine(const char *string);
private:
	void *outputfunction; ///< Holds pointer to the function which is used to output text

	/// \brief Indicates the type of console symbol.
	enum ConsoleSymbolType
	{
		ConsoleSymbolTypeConVar, ///< Indicates a console variable.
		ConsoleSymbolTypeConCmd ///< Indicates a console command.
	};

	/// \brief Used to wisely work with console symbol.
	union ConsoleSymbolPtr
	{
		ConVar *convar; ///< Used to work with console variable.
		ConCmd *concmd; ///< Used to work with console command.
	};

	/// \brief Describes a console symbol.
	/// \details Console symbol is a unified structure that console uses to keep track of 
	/// all variables and commands registered to it.
	struct ConsoleSymbol
	{
		char *name; ///< Name of the console symbol.
		ConsoleSymbolType type; ///< Type of the console symbol.
		ConsoleSymbolPtr *ptr; ///< Pointer to the underlying class.
		unsigned char numcmds; ///< If console symbol is a console command, this indicates the number of commands that share this symbol's name.
	};
	ConsoleSymbol *symbolbuffer; ///< Holds all registered console symbols.
	unsigned short symbolbuffersize; ///< Holds current size of the symbol buffer.
	unsigned short maxsymbolbuffersize; ///< Holds maximum size of the symbol buffer.
	unsigned char maxcmdspersymbol; ///< Holds maximum number of commands per 1 console symbol.
	char *commandbuffer; ///< Holds current command string.
	char **commandargs; ///< Holds arguments of current command.
	unsigned int argpos; ///< Holds starting index of first argument in the current command.
	unsigned char numargs; ///< Holds number of arguments in the current command.

	/// \brief Deletes console symbol from symbol buffer by it's index.
	/// \param[in] index Index of the console symbol.
	/// \return True on success, false otherwise.
	bool DeleteConsoleSymbolByIndex(const unsigned short index);

	/// \brief Deletes console variable from symbol buffer
	/// \param[in] name Name of the console variable.
	/// \note This function is used only by ConVar's destructor.
	/// \return No return.
	void DeleteConVar(const char *name);

	/// \brief Deletes console command from symbol buffer.
	/// \param[in] name Name of the console command.
	/// \param[in] ptr Pointer to the console command.
	/// \note This function is used only by ConCmd's destructor.
	/// \return No return.
	void DeleteConCmd(const char *name, ConCmd *ptr);

	/// \brief Returns console symbol by name.
	/// \param[in] name Name of the console symbol.
	/// \return Pointer to the console symbol on success, NULL otherwise.
	ConsoleSymbol *GetConsoleSymbol(const char *name);

	/// \brief Returns console symbol by it's index.
	/// \param[in] index Index of the console symbol.
	/// \return Pointer to the console symbol on success, NULL otherwise.
	ConsoleSymbol *GetConsoleSymbolByIndex(unsigned short index);

	/// \brief Retrieves symbol index by it's name.
	/// \param[in] name Name of the console symbol.
	/// \param[out] index Index of the console symbol.
	/// \return True on success, false otherwise.
	bool GetConsoleSymbolIndex(const char *name, unsigned short &index);

	/// \brief Returns help string of given symbol.
	/// \param[in] symbol Console symbol to work with.
	/// \return Help string on success, NULL otherwise.
	char *GetHelpString(const ConsoleSymbol *symbol);

	/// \brief Wrapper for realloc.
	/// \param[in,out] buffer Buffer to resize.
	/// \param[in] size Size to which resize.
	/// \return True on success, false otherwise.
	bool ResizeSymbolBuffer(ConsoleSymbol *&buffer, const unsigned short size);

	/// \brief Wrapper for realloc.
	/// \param[in,out] buffer Buffer to resize.
	/// \param[in] size Size to which resize.
	/// \return True on success, false otherwise.
	bool ResizeSymbolPtrBuffer(ConsoleSymbolPtr *&buffer, const unsigned char size);

	/// \brief Gets command string from multicommand string starting from startindex.
	/// \param[in] string Source input string to search in.
	/// \param[in] startindex Index to start search from.
	/// \return String containing 1 command on success, zero-length string otherwise.
	char *GetCommand(const char *string, const unsigned int startindex);

	/// \brief Interprets prepared command.
	/// \return No return.
	void InterpretCommand(void);
	friend class ConVar; ///< See ConVar.h
	friend class ConCmd; ///< See ConCmd.h
	friend void ConCmdCvarlist(ConsoleCore *concore, unsigned char numargs); ///< See coreconcommands.h
};
