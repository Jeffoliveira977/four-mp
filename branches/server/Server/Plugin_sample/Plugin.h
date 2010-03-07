#pragma once

#include "..\IPluginInterface.h"

class Plugin :
	public IPluginInterface
{
public:
	Plugin(void);
	~Plugin(void);
	virtual void OnPluginLoad(void);
	virtual void OnPluginUnload(void);
};
