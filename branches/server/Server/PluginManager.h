#pragma once

#include "IPluginInterface.h"

#ifdef WIN32
#include <windows.h>
#endif

class PluginManager
{
public:
	PluginManager(void);
	~PluginManager(void);
	unsigned char GetPluginBufferSize(void);
	bool LoadPlugin(const char *string);
private:
	struct Plugin
	{
		bool paused;
	#ifdef WIN32
		HMODULE module;
	#endif
		IPluginInterface *ptr;
		char *filename;
	};
	unsigned char maxpluginbuffersize;
	unsigned char pluginbuffersize;
	Plugin **pluginbuffer;
	bool LoadPluginInternal(const unsigned char index, const char *string);
	bool GetPluginFreeSlot(unsigned char &index);
	bool ResizePluginBuffer(Plugin **&buffer, const unsigned char size);
	void OnPluginLoad(const unsigned char index);
};
