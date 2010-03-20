#pragma once

#include "IPluginHandlerInterface.h"

#ifdef WIN32
#include <windows.h>
#endif

class PluginManager
{
public:
	PluginManager(void);
	~PluginManager(void);
	unsigned char GetMaxPluginBufferSize(void);
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
	IPluginHandlerInterface *GetPluginHandler(void);
private:
	short handleowneroffset;
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
	class PluginHandler : public IPluginHandlerInterface
	{
	public:
		PluginHandler(void);
		~PluginHandler(void);
		virtual void ReleaseAllHandleTypes(const IPluginInterface *plugin);
		virtual bool RequestNewHandleType(const IPluginInterface *plugin, unsigned short &index);
		virtual bool ReleaseHandleType(const IPluginInterface *plugin, const unsigned short &index);
		virtual void PrintToServer(const char *string, ...);
		virtual int CreateConVar(const IPluginInterface *plugin, const char *name, const float defaultvalue, const char *description = "", const int flags = 0, const bool hasMin = false, const float min = 0.0, const bool hasMax = false, const float max = 0.0);
		virtual int CreateConVar(const IPluginInterface *plugin, const char *name, const int defaultvalue, const char *description = "", const int flags = 0, const bool hasMin = false, const int min = 0, const bool hasMax = false, const int max = 0);
		virtual int CreateConVar(const IPluginInterface *plugin, const char *name, const char *defaultvalue, const char *description = "", const int flags = 0);
		virtual int FindConVar(const IPluginInterface *plugin, const char *name);
		virtual bool ResetConVar(const IPluginInterface *plugin, const int handle);
		virtual char *GetConVarName(const IPluginInterface *plugin, const int handle);
		virtual bool GetConVarValue(const IPluginInterface *plugin, const int handle, float &value);
		virtual bool GetConVarValue(const IPluginInterface *plugin, const int handle, int &value);
		virtual bool GetConVarValue(const IPluginInterface *plugin, const int handle, char *&value);
		virtual bool GetConVarFlags(const IPluginInterface *plugin, const int handle, int &flags);
		virtual bool GetConVarBound(const IPluginInterface *plugin, const int handle, const ConVarBoundType type, float &bound);
		virtual bool GetConVarBound(const IPluginInterface *plugin, const int handle, const ConVarBoundType type, int &bound);
		virtual bool SetConVarValue(const IPluginInterface *plugin, const int handle, const float value);
		virtual bool SetConVarValue(const IPluginInterface *plugin, const int handle, const int value);
		virtual bool SetConVarValue(const IPluginInterface *plugin, const int handle, const char *value);
		virtual bool SetConVarFlags(const IPluginInterface *plugin, const int handle, const int flags);
		virtual bool SetConVarBound(const IPluginInterface *plugin, const int handle, const ConVarBoundType, const float bound);
		virtual bool SetConVarBound(const IPluginInterface *plugin, const int handle, const ConVarBoundType, const int bound);
		virtual void RegServerCmd(const IPluginInterface *plugin, const char *name, void *callback, const char *description = "", const int flags = 0);
		virtual unsigned char GetCmdArgs(void);
		virtual bool GetCmdArgString(char *&arg);
		virtual bool GetCmdArgType(const unsigned char argnum, ConVarType &type);
		virtual bool GetCmdArg(const unsigned char argnum, char *&arg);
		virtual bool GetCmdArg(const unsigned char argnum, int &arg);
		virtual bool GetCmdArg(const unsigned char argnum, float &arg);
		virtual void ServerCommand(const char *string);
	};
	IPluginHandlerInterface *ph;
	bool LoadPluginInternal(const unsigned char index, const char *string);
	unsigned char GetNumberOfFreePluginSlots(void);
	bool GetPluginFreeSlot(unsigned char &index);
	bool FindPlugin(const IPluginInterface *plugin, unsigned char &index);
	bool ResizePluginBuffer(Plugin **&buffer, const unsigned char size);
	void OnPluginLoad(const unsigned char index);
	void OnPluginUnload(const unsigned char index);
};
