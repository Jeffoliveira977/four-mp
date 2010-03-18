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
	virtual unsigned char GetCmdArgs(void) = 0;
	virtual bool GetCmdArgString(char *&arg) = 0;
	virtual bool GetCmdArgType(const unsigned char argnum, ConVarType &type) = 0;
	virtual bool GetCmdArg(const unsigned char argnum, char *&arg) = 0;
	virtual bool GetCmdArg(const unsigned char argnum, int &arg) = 0;
	virtual bool GetCmdArg(const unsigned char argnum, float &arg) = 0;
	virtual void RegServerCmd(const IPluginInterface *plugin, const char *name, void *callback, const char *description = "", const int flags = 0) = 0;
	virtual void ServerCommand(const char *string) = 0;
};