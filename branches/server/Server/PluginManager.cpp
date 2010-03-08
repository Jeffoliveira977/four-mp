#include <stdio.h>
#include <string.h>
#include <io.h>

#include "PluginManager.h"
#include "console\ConsoleScreen.h"

extern ConsoleScreen conscreen;
extern PluginManager pm;

PluginManager::PluginManager(void)
{
	maxpluginbuffersize = 8;
	pluginbuffersize = 0;
	ph = new PluginHandler;
}

PluginManager::~PluginManager(void)
{
	this->UnloadPlugins();
	free(pluginbuffer);
	delete ph;
}

unsigned char PluginManager::GetPluginBufferSize(void)
{
	return pluginbuffersize;
}

void PluginManager::LoadPlugins(void)
{
	intptr_t ptr;
	_finddata64i32_t data;
	ptr = _findfirst("plugins\\*.dll", &data);
	if (ptr == -1)
	{
		return;
	}
	int continuesearch = 0;
	unsigned char slots = this->GetNumberOfFreePluginSlots();
	unsigned char i = 0;
	while ((continuesearch == 0) && (i < slots))
	{
		if ((!this->IsPluginLoaded(data.name)) && (this->LoadPlugin(data.name)))
		{
			i++;
		}
		continuesearch = _findnext(ptr, &data);
	}
	_findclose(ptr);
}

void PluginManager::UnloadPlugins(void)
{
	for (unsigned char i = 0; i < pluginbuffersize; i++)
	{
		if (pluginbuffer[i] != NULL)
		{
			this->UnloadPlugin(i);
		}
	}
}

void PluginManager::ReloadPlugins(void)
{
	for (unsigned char i = 0; i < pluginbuffersize; i++)
	{
		if (pluginbuffer[i] != NULL)
		{
			this->ReloadPlugin(i);
		}
	}
}

void PluginManager::PausePlugins(void)
{
	for (unsigned char i = 0; i < pluginbuffersize; i++)
	{
		if (pluginbuffer[i] != NULL)
		{
			this->PausePlugin(i);
		}
	}
}

void PluginManager::UnpausePlugins(void)
{
	for (unsigned char i = 0; i < pluginbuffersize; i++)
	{
		if (pluginbuffer[i] != NULL)
		{
			this->UnpausePlugin(i);
		}
	}
}

bool PluginManager::IsPluginLoaded(const char *string)
{
	char *plugin = (char *)calloc(strlen(string) + 8, sizeof(char));
	sprintf(plugin, "plugins/%s", string);
	for (unsigned char i = 0; i < pluginbuffersize; i++)
	{
		if ((pluginbuffer[i] != NULL) && (strcmp(pluginbuffer[i]->filename, plugin) == 0))
		{
			return true;
		}
	}
	return false;
}

bool PluginManager::LoadPlugin(const char *string)
{
	unsigned char index;
	if (!this->GetPluginFreeSlot(index))
	{
		return false;
	}
	char *plugin = (char *)calloc(strlen(string) + 8, sizeof(char));
	sprintf(plugin, "plugins/%s", string);
	if (!this->LoadPluginInternal(index, plugin))
	{
		return false;
	}
	return true;
}

bool PluginManager::UnloadPlugin(const unsigned char index)
{
	if (index >= pluginbuffersize)
	{
		return false;
	}
	if (pluginbuffer[index] == NULL)
	{
		return false;
	}
	this->OnPluginUnload(index);
	pluginbuffer[index]->paused = false;
	FreeLibrary(pluginbuffer[index]->module);
	free(pluginbuffer[index]->filename);
	delete pluginbuffer[index];
	pluginbuffer[index] = NULL;
	return true;
}

bool PluginManager::ReloadPlugin(const unsigned char index)
{
	if (index >= pluginbuffersize)
	{
		return false;
	}
	if (pluginbuffer[index] == NULL)
	{
		return false;
	}
	char *filename = (char *)calloc(strlen(pluginbuffer[index]->filename) + 1, sizeof(char));
	strcpy(filename, pluginbuffer[index]->filename);
	if (!this->UnloadPlugin(index))
	{
		return false;
	}
	if (!this->LoadPluginInternal(index, filename))
	{
		return false;
	}
	return true;
}

bool PluginManager::PausePlugin(const unsigned char index)
{
	if (index >= pluginbuffersize)
	{
		return false;
	}
	if (pluginbuffer[index] == NULL)
	{
		return false;
	}
	if (pluginbuffer[index]->paused)
	{
		return false;
	}
	pluginbuffer[index]->paused = true;
	return true;
}

bool PluginManager::UnpausePlugin(const unsigned char index)
{
	if (index >= pluginbuffersize)
	{
		return false;
	}
	if (pluginbuffer[index] == NULL)
	{
		return false;
	}
	if (!pluginbuffer[index]->paused)
	{
		return false;
	}
	pluginbuffer[index]->paused = false;
	return true;
}

bool PluginManager::GetPluginInfoString(const unsigned char index, char *&string)
{
	if (index >= pluginbuffersize)
	{
		return false;
	}
	if (pluginbuffer[index] == NULL)
	{
		return false;
	}
	char *name = pluginbuffer[index]->ptr->GetName();
	char *version = pluginbuffer[index]->ptr->GetVersion();
	char *author = pluginbuffer[index]->ptr->GetAuthor();
	if (!pluginbuffer[index]->paused)
	{
		string = (char *)calloc(_scprintf("%d \"%s\" (%s) by %s", index, name, version, author) + 1, sizeof(char));
		sprintf(string, "%d \"%s\" (%s) by %s", index, name, version, author);
	}
	else
	{
		string = (char *)calloc(_scprintf("%d (Paused) \"%s\" (%s) by %s", index, name, version, author) + 1, sizeof(char));
		sprintf(string, "%d (Paused) \"%s\" (%s) by %s", index, name, version, author);
	}
	return true;
}

IPluginHandlerInterface *PluginManager::GetPluginHandler(void)
{
	return ph;
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
	pluginbuffer[index]->filename = (char *)calloc(strlen(string) + 1, sizeof(char));
	strcpy(pluginbuffer[index]->filename, string);
	this->OnPluginLoad(index);
	return true;
}

unsigned char PluginManager::GetNumberOfFreePluginSlots(void)
{
	unsigned char slots = 0;
	for (unsigned char i = 0; i < pluginbuffersize; i++)
	{
		if (pluginbuffer[i] == NULL)
		{
			slots++;
		}
	}
	slots = maxpluginbuffersize - pluginbuffersize + slots;
	return slots;
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

void PluginManager::OnPluginUnload(const unsigned char index)
{
	if (index >= pluginbuffersize)
	{
		return;
	}
	if (pluginbuffer[index] == NULL)
	{
		return;
	}
	pluginbuffer[index]->ptr->OnPluginUnload();
}

extern "C" __declspec(dllexport) IPluginHandlerInterface *GetPluginHandler(void)
{
	return pm.GetPluginHandler();
}

PluginManager::PluginHandler::PluginHandler(void)
{
}

PluginManager::PluginHandler::~PluginHandler(void)
{
}

void PluginManager::PluginHandler::PrintToServer(const char *string)
{
	conscreen.Print(string);
}