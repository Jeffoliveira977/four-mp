#pragma once

#include "console\ConVar.h"
#include "IPluginInterface.h"

class IPluginHandlerInterface
{
public:
	//Handles
	virtual void ReleaseAllHandleTypes(const IPluginInterface *plugin) = 0;
	virtual bool RequestNewHandleType(const IPluginInterface *plugin, unsigned short &index) = 0;
	virtual bool ReleaseHandleType(const IPluginInterface *plugin, const unsigned short &index) = 0;
	//Console
	virtual void PrintToServer(const char *string, ...) = 0;
	virtual int CreateConVar(const IPluginInterface *plugin, const char *name, const float defaultvalue, const char *description = "", const int flags = 0, const bool hasMin = false, const float min = 0.0, const bool hasMax = false, const float max = 0.0) = 0;
	virtual int CreateConVar(const IPluginInterface *plugin, const char *name, const int defaultvalue, const char *description = "", const int flags = 0, const bool hasMin = false, const int min = 0, const bool hasMax = false, const int max = 0) = 0;
	virtual int CreateConVar(const IPluginInterface *plugin, const char *name, const char *defaultvalue, const char *description = "", const int flags = 0) = 0;
	virtual int FindConVar(const IPluginInterface *plugin, const char *name) = 0;
	virtual bool ResetConVar(const IPluginInterface *plugin, const int handle) = 0;
	virtual char *GetConVarName(const IPluginInterface *plugin, const int handle) = 0;
	virtual bool GetConVarValue(const IPluginInterface *plugin, const int handle, float &value) = 0;
	virtual bool GetConVarValue(const IPluginInterface *plugin, const int handle, int &value) = 0;
	virtual bool GetConVarValue(const IPluginInterface *plugin, const int handle, char *&value) = 0;
	virtual bool GetConVarFlags(const IPluginInterface *plugin, const int handle, int &flags) = 0;
	virtual bool GetConVarBound(const IPluginInterface *plugin, const int handle, const ConVarBoundType type, float &bound) = 0;
	virtual bool GetConVarBound(const IPluginInterface *plugin, const int handle, const ConVarBoundType type, int &bound) = 0;
	virtual bool SetConVarValue(const IPluginInterface *plugin, const int handle, const float value) = 0;
	virtual bool SetConVarValue(const IPluginInterface *plugin, const int handle, const int value) = 0;
	virtual bool SetConVarValue(const IPluginInterface *plugin, const int handle, const char *value) = 0;
	virtual bool SetConVarFlags(const IPluginInterface *plugin, const int handle, const int flags) = 0;
	virtual bool SetConVarBound(const IPluginInterface *plugin, const int handle, const ConVarBoundType, const float bound) = 0;
	virtual bool SetConVarBound(const IPluginInterface *plugin, const int handle, const ConVarBoundType, const int bound) = 0;
	virtual void RegServerCmd(const IPluginInterface *plugin, const char *name, void *callback, const char *description = "", const int flags = 0) = 0;
	virtual unsigned char GetCmdArgs(void) = 0;
	virtual bool GetCmdArgString(char *&arg) = 0;
	virtual bool GetCmdArgType(const unsigned char argnum, ConVarType &type) = 0;
	virtual bool GetCmdArg(const unsigned char argnum, char *&arg) = 0;
	virtual bool GetCmdArg(const unsigned char argnum, int &arg) = 0;
	virtual bool GetCmdArg(const unsigned char argnum, float &arg) = 0;
	virtual void ServerCommand(const char *string) = 0;
};