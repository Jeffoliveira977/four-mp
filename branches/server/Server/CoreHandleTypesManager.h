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
	bool CloseHandle(const int index);
	bool AddScriptCommand(const short owner, const char *callback, const ConCmd *ptr);
	bool DeleteScriptCommand(const int index);
	bool Execute(const int index, const unsigned char numargs);
private:
	struct ScriptCommand
	{
		int index;
		char *callback;
	};
	ScriptCommand *commandbuffer;
	unsigned short maxcommandbuffersize;
	unsigned short commandbuffersize;
	bool GetScriptCommandCallback(const int index, char *&callback);
	bool ResizeCommandBuffer(ScriptCommand *&buffer, const unsigned short size);
};
