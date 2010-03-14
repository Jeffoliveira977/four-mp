#include <stdlib.h>
#include <string.h>

#include "CoreHandleTypesManager.h"
#include "HandleManager.h"
#include "VirtualMachineManager.h"

extern HandleManager hm;
extern ConsoleCore concore;
extern VirtualMachineManager vmm;

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
			delete ((ConVar *)hm.handlebuffer[index]->ptr);
			break;
		}
	case HandleTypeConCmd:
		{
			delete ((ConCmd *)hm.handlebuffer[index]->ptr);
			break;
		}
	}
	return true;
}

int *CoreHandleTypesManager::GetDynamicCommandHandles(const char *name, unsigned char &numcmds)
{
	if (name == NULL)
	{
		return NULL;
	}
	numcmds = 0;
	int *handles = NULL;
	for (unsigned short i = 0; i < commandbuffersize; i++)
	{
		if (strcmp(((ConCmd *)hm.handlebuffer[commandbuffer[i].index]->ptr)->GetName(), name) == 0)
		{
			if (!this->ResizeHandleIndexBuffer(handles, numcmds + 1))
			{
				return NULL;
			}
			handles[numcmds] = commandbuffer[i].index;
			numcmds++;
		}
	}
	if (numcmds == 0)
	{
		return NULL;
	}
	return handles;
}

bool CoreHandleTypesManager::AddDynamicCommand(const short owner, const char *callback, const ConCmd *ptr)
{
	if (ptr == NULL)
	{
		return false;
	}
	if ((owner < 0) || (owner > hm.maxcountbuffersize))
	{
		return false;
	}
	if (commandbuffersize == maxcommandbuffersize)
	{
		return false;
	}
	int index = hm.FindHandle(ptr);
	if (index != INVALID_HANDLE)
	{
		return false;
	}
	index = hm.AddNewHandle(owner, HandleTypeConCmd, (void *)ptr);
	if (index == INVALID_HANDLE)
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

bool CoreHandleTypesManager::DeleteDynamicCommand(const int index)
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

bool CoreHandleTypesManager::ExecuteDynamicCommand(const int index, const unsigned char numargs)
{
	char *callback;
	if (!this->GetDynamicCommandCallback(index, callback))
	{
		return false;
	}
	for (short i = 0; i < hm.handlebuffer[index]->numowners; i++)
	{
		if ((hm.handlebuffer[index]->owner[i] > 0) && (hm.handlebuffer[index]->owner[i] < hm.pluginowneroffset))
		{
			vmm.FireCommandCallback(hm.handlebuffer[index]->owner[i] - 1, callback, numargs);
		}
		else if (hm.handlebuffer[index]->owner[i] >= hm.pluginowneroffset)
		{
			//Redirect to PluginManager;
		}
	}
	free(callback);
	return true;
}

bool CoreHandleTypesManager::ResizeHandleIndexBuffer(int *&buffer, const unsigned char size)
{
	int *tempbuffer = (int *)realloc(buffer, size * sizeof(int));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

bool CoreHandleTypesManager::GetDynamicCommandCallback(const int index, char *&callback)
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

bool CoreHandleTypesManager::ResizeCommandBuffer(DynamicCommand *&buffer, const unsigned short size)
{
	DynamicCommand *tempbuffer = (DynamicCommand *)realloc(buffer, size * sizeof(DynamicCommand));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}