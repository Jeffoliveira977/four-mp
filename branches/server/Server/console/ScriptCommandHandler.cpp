#include <stdlib.h>
#include <string.h>

#include "..\sq\squirrel.h"
#include "..\sq\sqstdsystem.h"
#include "ScriptCommandHandler.h"

//extern HSQUIRRELVM v;

ScriptCommandHandler::ScriptCommandHandler(void)
{
	maxcommandbuffersize = 65535;

	commandbuffersize = 0;
}

ScriptCommandHandler::~ScriptCommandHandler(void)
{
	for (unsigned short i = 0; i < commandbuffersize; i++)
	{
		free(commandbuffer[i].name);
		free(commandbuffer[i].callback);
	}
}

bool ScriptCommandHandler::IsScriptCommandExist(const char *name)
{
	for (unsigned short i = 0; i < commandbuffersize; i++)
	{
		if ((strcmp(commandbuffer[i].name, name)) == 0)
		{
			return true;
		}
	}
	return false;
}

bool ScriptCommandHandler::AddScriptCommand(const char *name, const char *callback)
{
	if (commandbuffersize == maxcommandbuffersize)
	{
		return false;
	}
	if (this->IsScriptCommandExist(name))
	{
		return false;
	}
	commandbuffersize++;
	if (!this->ResizeCommandBuffer(commandbuffer, commandbuffersize))
	{
		return false;
	}
	ScriptCommand *command = (ScriptCommand *)calloc(1, sizeof(ScriptCommand));
	command->name = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(command->name, name);
	command->callback = (char *)calloc(strlen(callback) + 1, sizeof(char));
	strcpy(command->callback, callback);
	commandbuffer[commandbuffersize-1] = *command;
	return true;
}

bool ScriptCommandHandler::DeleteScriptCommand(const char *name)
{
	for (unsigned short i = 0; i < commandbuffersize; i++)
	{
		if ((strcmp(commandbuffer[i].name, name)) == 0)
		{
			free(commandbuffer[i].name);
			free(commandbuffer[i].callback);
			return true;
		}
	}
	return false;
}

bool ScriptCommandHandler::Execute(const char *name, const unsigned char numargs)
{
	char *callback;
	if (!this->GetScriptCommandCallback(name, callback))
	{
		return false;
	}
	int result;
	//int top = sq_gettop(v); 
	//sq_pushroottable(v);
	//sq_pushstring(v,_SC(callback),-1);
	//if(SQ_SUCCEEDED(sq_get(v,-2))) { 
	//	sq_pushroottable(v); 
	//	sq_pushinteger(v,numargs); 
	//	sq_call(v,2,0,0);
	//}
	//sq_settop(v,top);
	free(callback);
	return true;
}

bool ScriptCommandHandler::GetScriptCommandCallback(const char *name, char *&callback)
{
	for (unsigned short i = 0; i < commandbuffersize; i++)
	{
		if ((strcmp(commandbuffer[i].name, name)) == 0)
		{
			callback = (char *)calloc(strlen(commandbuffer[i].callback) + 1, sizeof(char));
			strcpy(callback, commandbuffer[i].callback);
			return true;
		}
	}
	return false;
}

bool ScriptCommandHandler::ResizeCommandBuffer(ScriptCommand *&buffer, const unsigned short size)
{
	ScriptCommand *tempbuffer = (ScriptCommand *)realloc(buffer, size * sizeof(ScriptCommand));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}