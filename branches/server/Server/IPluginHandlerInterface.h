#pragma once

#include "IPluginInterface.h"

class IPluginHandlerInterface
{
public:
	virtual void ReleaseAllHandleTypes(const IPluginInterface *plugin) = 0;
	virtual bool RequestNewHandleType(const IPluginInterface *plugin, unsigned short &index) = 0;
	virtual bool ReleaseHandleType(const IPluginInterface *plugin, const unsigned short &index) = 0;
	virtual void PrintToServer(const char *string, ...) = 0;
};