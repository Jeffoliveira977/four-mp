#pragma once

#include "console\ConsoleCore.h"

enum CoreHandleType
{
	HandleTypeConVar,
	HandleTypeConCmd
};

#define NUM_CORE_HANDLE_TYPES 2

class CoreHandleTypesManager
{
public:
	CoreHandleTypesManager(void);
	~CoreHandleTypesManager(void);
	bool CloseHandle(const int handle); //Wisely closes given handle since HandleManager doesn't know what data it operates on
	int FindConVar(const short owner, const char *name); //Returns handle to the ConVar and reserves it by owner
	bool ResetConVar(const short owner, const int handle); //Resets the console variable to its default value
	char *GetConVarName(const short owner, const int handle); //Retrieves the name of a console variable
	bool GetConVarValue(const short owner, const int handle, float &value);
	bool GetConVarValue(const short owner, const int handle, int &value);
	bool GetConVarValue(const short owner, const int handle, char *&value);
	bool GetConVarFlags(const short owner, const int handle, int &flags);
	bool GetConVarBound(const short owner, const int handle, const ConVarBoundType type, float &bound);
	bool GetConVarBound(const short owner, const int handle, const ConVarBoundType type, int &bound);
	bool SetConVarValue(const short owner, const int handle, const float value);
	bool SetConVarValue(const short owner, const int handle, const int value);
	bool SetConVarValue(const short owner, const int handle, const char *value);
	bool SetConVarFlags(const short owner, const int handle, const int flags);
	bool SetConVarBound(const short owner, const int handle, const ConVarBoundType, const float bound);
	bool SetConVarBound(const short owner, const int handle, const ConVarBoundType, const int bound);
	bool AddDynamicCommand(const short owner, const char *callback, const ConCmd *ptr); //Adds dynamic command. It is console command created by filterscript
	int *GetDynamicCommandHandles(const char *name, unsigned char &numcmds); //Returns handle index of dynamic command
	bool DeleteDynamicCommand(const int handle); //Deletes dynamic command by given handle index
	bool ExecuteDynamicCommand(const int handle, const unsigned char numargs); //Executes dynamic command by given handle index
private:
	struct DynamicCommand
	{
		int index; //Holds handle index of current dynamic command
		char *callback; //Holds callback of current dynamic command
	};
	DynamicCommand *commandbuffer; //Holds all additional data needed by dynamic commands
	unsigned short maxcommandbuffersize; //Holds maximum size of dynamic commands buffer
	unsigned short commandbuffersize; //Holds current size of dynamic commands buffer
	bool ResizeHandleIndexBuffer(int *&buffer, const unsigned char size); //Wrapper for realloc
	bool CheckConVar(const short owner, const int handle); //Checks if given handle is a ConVar and owned by given owner
	bool GetDynamicCommandCallback(const int handle, char *&callback); //Returns callback of given handle index
	bool ResizeCommandBuffer(DynamicCommand *&buffer, const unsigned short size); //Wrapper for realloc
};
