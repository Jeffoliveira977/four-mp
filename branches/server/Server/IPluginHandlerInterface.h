#pragma once

#include "IPluginInterface.h"

class IPluginHandlerInterface
{
public:
	virtual void PrintToServer(const char *string) = 0;
};