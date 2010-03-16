#pragma once

#include "..\IPluginHandlerInterface.h"

class PluginCore :
	public IPluginInterface
{
public:
	PluginCore(void);
	~PluginCore(void);
	virtual bool AttachToServer(void);
	virtual void OnPluginLoad(void);
	virtual void OnPluginUnload(void);
	virtual char *GetName(void);
	virtual char *GetVersion(void);
	virtual char *GetAuthor(void);
protected:
	IPluginHandlerInterface *server;
};
