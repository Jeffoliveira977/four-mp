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