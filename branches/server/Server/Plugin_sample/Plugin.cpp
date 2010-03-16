#include "Plugin.h"

Plugin plugin;
IPluginInterface *pluginptr = &plugin;

Plugin::Plugin(void)
{
}

Plugin::~Plugin(void)
{
}

void Plugin::OnPluginLoad(void)
{
	//Your code here
	server->PrintToServer("HELLO WORLD!");
}

void Plugin::OnPluginUnload(void)
{
	//Your code here
}