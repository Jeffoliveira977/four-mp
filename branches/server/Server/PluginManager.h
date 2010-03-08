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
	void LoadPlugins(void);
	void UnloadPlugins(void);
	void ReloadPlugins(void);
	void PausePlugins(void);
	void UnpausePlugins(void);
	bool IsPluginLoaded(const char *string);
	bool LoadPlugin(const char *string);
	bool UnloadPlugin(const unsigned char index);
	bool ReloadPlugin(const unsigned char index);
	bool PausePlugin(const unsigned char index);
	bool UnpausePlugin(const unsigned char index);
	bool GetPluginInfoString(const unsigned char index, char *&string);
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
	unsigned char GetNumberOfFreePluginSlots(void);
	bool GetPluginFreeSlot(unsigned char &index);
	bool ResizePluginBuffer(Plugin **&buffer, const unsigned char size);
	void OnPluginLoad(const unsigned char index);
	void OnPluginUnload(const unsigned char index);
};
