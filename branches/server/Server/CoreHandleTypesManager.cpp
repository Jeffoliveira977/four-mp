#include <stdlib.h>
#include <string.h>

#include "CoreHandleTypesManager.h"
#include "HandleManager.h"

extern HandleManager hm;
extern ConsoleCore concore;

CoreHandleTypesManager::CoreHandleTypesManager(void)
{
	maxcommandbuffersize = 65535;
	commandbuffersize = 0;
}

CoreHandleTypesManager::~CoreHandleTypesManager(void)
{
	for (unsigned short i = 0; i < commandbuffersize; i++)
	{
		free(commandbuffer[i].callback);
	}
}

bool CoreHandleTypesManager::CloseHandle(const int index)
{
	if ((index < 0) || (index >= hm.handlebuffersize))
	{
		return false;
	}
	if (hm.handlebuffer[index] == NULL)
	{
		return false;
	}
	if (hm.handlebuffer[index]->numowners != 0)
	{
		return false;
	}
	if (hm.handlebuffer[index]->type >= NUM_CORE_HANDLE_TYPES)
	{
		return false;
	}
	switch (hm.handlebuffer[index]->type)
	{
	case HandleTypeConVar:
		{
			if (!concore.DeleteConsoleSymbol(((ConVar *)hm.handlebuffer[index]->ptr)->GetName()))
			{
				return false;
			}
			break;
		}
	case HandleTypeConCmd:
		{
			if (!concore.DeleteConsoleSymbol(((ConCmd *)hm.handlebuffer[index]->ptr)->GetName()))
			{
				return false;
			}
			break;
		}
	}
	return true;
}

bool CoreHandleTypesManager::AddScriptCommand(const short owner, const char *callback, const ConCmd *ptr)
{
	if (ptr == NULL)
	{
		return false;
	}
	if ((owner < 0) || (owner >= hm.maxcountbuffersize))
	{
		return false;
	}
	if (commandbuffersize == maxcommandbuffersize)
	{
		return false;
	}
	int index = hm.FindHandle(ptr);
	if (index != -1)
	{
		return false;
	}
	index = hm.AddNewHandle(owner, HandleTypeConCmd, (void *)ptr);
	if (index != -1)
	{
		return false;
	}
	commandbuffersize++;
	if (!this->ResizeCommandBuffer(commandbuffer, commandbuffersize))
	{
		return false;
	}
	commandbuffer[commandbuffersize-1].index = index;
	commandbuffer[commandbuffersize-1].callback = (char *)calloc(strlen(callback) + 1, sizeof(char));
	strcpy(commandbuffer[commandbuffersize-1].callback, callback);
	return true;
}

bool CoreHandleTypesManager::DeleteScriptCommand(const int index)
{
	for (unsigned short i = 0; i < commandbuffersize; i++)
	{
		if (commandbuffer[i].index == index)
		{
			free(commandbuffer[i].callback);
			return true;
		}
	}
	return false;
}

bool CoreHandleTypesManager::Execute(const int index, const unsigned char numargs)
{
	char *callback;
	if (!this->GetScriptCommandCallback(index, callback))
	{
		return false;
	}
	//TODO
	//vmm.FireCommandCallback(...);
	free(callback);
	return true;
}

bool CoreHandleTypesManager::GetScriptCommandCallback(const int index, char *&callback)
{
	for (unsigned short i = 0; i < commandbuffersize; i++)
	{
		if (commandbuffer[i].index == index)
		{
			callback = (char *)calloc(strlen(commandbuffer[i].callback) + 1, sizeof(char));
			strcpy(callback, commandbuffer[i].callback);
			return true;
		}
	}
	return false;
}

bool CoreHandleTypesManager::ResizeCommandBuffer(ScriptCommand *&buffer, const unsigned short size)
{
	ScriptCommand *tempbuffer = (ScriptCommand *)realloc(buffer, size * sizeof(ScriptCommand));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}