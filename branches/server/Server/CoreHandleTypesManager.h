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
	bool CloseHandle(const int index); //Wisely closes given handle since HandleManager doesn't know what data it operates on
	bool AddDynamicCommand(const short owner, const char *callback, const ConCmd *ptr); //Adds dynamic command. It is console command created by plugin or filterscript
	int *GetDynamicCommandHandles(const char *name, unsigned char &numcmds); //Returns handle index of dynamic command
	bool DeleteDynamicCommand(const int index); //Deletes dynamic command by given handle index
	bool ExecuteDynamicCommand(const int index, const unsigned char numargs); //Executes dynamic command by given handle index
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
	bool GetDynamicCommandCallback(const int index, char *&callback); //Returns callback of given handle index
	bool ResizeCommandBuffer(DynamicCommand *&buffer, const unsigned short size); //Wrapper for realloc
};
