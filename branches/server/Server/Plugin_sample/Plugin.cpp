#include "Plugin.h"

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
}