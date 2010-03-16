#pragma once

#include "PluginCore.h"

class Plugin :
	public PluginCore
{
public:
	Plugin(void);
	~Plugin(void);
	virtual void OnPluginLoad(void);
	virtual void OnPluginUnload(void);
};
