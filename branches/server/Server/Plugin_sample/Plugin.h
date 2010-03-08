#pragma once

#include "..\IPluginHandlerInterface.h"

class Plugin :
	public IPluginInterface
{
public:
	Plugin(void);
	~Plugin(void);
	virtual void OnPluginLoad(void);
	virtual void OnPluginUnload(void);
	virtual char *GetName(void);
	virtual char *GetVersion(void);
	virtual char *GetAuthor(void);
private:
	IPluginHandlerInterface *ph;
};
