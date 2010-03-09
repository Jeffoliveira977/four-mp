#include "Plugin.h"

#ifdef WIN32
#include "windows.h"
#endif

Plugin plugin;
IPluginInterface *pluginptr = &plugin;

extern "C" __declspec(dllexport) IPluginInterface *GetPlugin(void)
{
	return pluginptr;
}

Plugin::Plugin(void)
{
}

Plugin::~Plugin(void)
{
}

void Plugin::OnPluginLoad(void)
{
	typedef IPluginHandlerInterface *(*GetPluginHandlerFunctionType)();
	GetPluginHandlerFunctionType GetPluginHandlerFunction = NULL;
	GetPluginHandlerFunction = (GetPluginHandlerFunctionType)GetProcAddress(GetModuleHandle(NULL), "GetPluginHandler");
	if (GetPluginHandlerFunction == NULL)
	{
		return;
	}
	ph = GetPluginHandlerFunction();
	ph->PrintToServer("HELLO WORLD!");
	unsigned short handletypeindex = 0;
	if (ph->RequestNewHandleType(this, handletypeindex))
	{
		ph->PrintToServer("New handle type: %d", handletypeindex);
	}
}

void Plugin::OnPluginUnload(void)
{
}

char *Plugin::GetName(void)
{
	return "Sample plugin";
}

char *Plugin::GetVersion(void)
{
	return "0.0.0.0";
}

char *Plugin::GetAuthor(void)
{
	return "FOUR-MP team";
}