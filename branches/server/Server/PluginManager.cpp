#include <stdio.h>
#include <string.h>

#include "PluginManager.h"

PluginManager::PluginManager(void)
{
	maxpluginbuffersize = 8;
	pluginbuffersize = 0;
}

PluginManager::~PluginManager(void)
{
}

unsigned char PluginManager::GetPluginBufferSize(void)
{
	return pluginbuffersize;
}
bool PluginManager::LoadPlugin(const char *string)
{
	unsigned char index;
	if (!this->GetPluginFreeSlot(index))
	{
		return false;
	}
	int length = strlen(string) + 8;
	char *plugin = (char *)calloc(length, sizeof(char));
	sprintf(plugin, "plugins/%s", string);
	if (!this->LoadPluginInternal(index, plugin))
	{
		return false;
	}
	pluginbuffer[index]->filename = (char *)calloc(length, sizeof(char));
	strcpy(pluginbuffer[index]->filename, string);
	this->OnPluginLoad(index);
	return true;
}

bool PluginManager::LoadPluginInternal(const unsigned char index, const char *string)
{
	if (index >= maxpluginbuffersize)
	{
		return false;
	}
	if (index >= pluginbuffersize)
	{
		if (pluginbuffersize == maxpluginbuffersize)
		{
			return false;
		}
		if (!this->ResizePluginBuffer(pluginbuffer, index + 1))
		{
			return false;
		}
		pluginbuffer[index] = NULL;
		pluginbuffersize = index + 1;
	}
	if (pluginbuffer[index] != NULL)
	{
		return false;
	}
	pluginbuffer[index] = new Plugin;
	pluginbuffer[index]->module = LoadLibrary(string);
	if (pluginbuffer[index]->module == NULL)
	{
		delete pluginbuffer[index];
		pluginbuffer[index] = NULL;
		return false;
	}
	typedef IPluginInterface *(*GetPluginFunctionType)();
	GetPluginFunctionType GetPluginFunction = NULL;
	GetPluginFunction = (GetPluginFunctionType)GetProcAddress(pluginbuffer[index]->module, "GetPlugin");
	if (GetPluginFunction == NULL)
	{
		FreeLibrary(pluginbuffer[index]->module);
		delete pluginbuffer[index];
		pluginbuffer[index] = NULL;
		return false;
	}
	pluginbuffer[index]->ptr = GetPluginFunction();
	pluginbuffer[index]->paused = false;
	return true;
}

bool PluginManager::GetPluginFreeSlot(unsigned char &index)
{
	for (index = 0; index < pluginbuffersize; index++)
	{
		if (pluginbuffer[index] == NULL)
		{
			return true;
		}
	}
	if (pluginbuffersize == maxpluginbuffersize)
	{
		return false;
	}
	return true;
}

bool PluginManager::ResizePluginBuffer(PluginManager::Plugin **&buffer, const unsigned char size)
{
	Plugin **tempbuffer = (Plugin **)realloc(*&buffer, size * sizeof(Plugin *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

void PluginManager::OnPluginLoad(const unsigned char index)
{
	if (index >= pluginbuffersize)
	{
		return;
	}
	if (pluginbuffer[index] == NULL)
	{
		return;
	}
	pluginbuffer[index]->ptr->OnPluginLoad();
}