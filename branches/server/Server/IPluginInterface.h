#pragma once

class IPluginInterface
{
public:
	virtual void OnPluginLoad(void) = 0;
	virtual void OnPluginUnload(void) = 0;
};
