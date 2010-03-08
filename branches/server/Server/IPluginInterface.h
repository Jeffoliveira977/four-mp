#pragma once

class IPluginInterface
{
public:
	virtual void OnPluginLoad(void) = 0;
	virtual void OnPluginUnload(void) = 0;
	virtual char *GetName(void) = 0;
	virtual char *GetVersion(void) = 0;
	virtual char *GetAuthor(void) = 0;
};
