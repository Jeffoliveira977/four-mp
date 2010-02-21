#pragma once

class ScriptCommandHandler
{
public:
	ScriptCommandHandler(void);
	~ScriptCommandHandler(void);
	bool IsScriptCommandExist(const char *name);
	bool AddScriptCommand(const char *name, const char *callback);
	bool DeleteScriptCommand(const char *name);
	bool Execute(const char *name, const unsigned char numargs);
private:
	struct ScriptCommand
	{
		char *name;
		char *callback;
	};
	ScriptCommand *commandbuffer;
	unsigned short commandbuffersize;
	unsigned short maxcommandbuffersize;
	bool GetScriptCommandCallback(const char *name, char *&callback);
	bool ResizeCommandBuffer(ScriptCommand *&buffer, const unsigned short size);
};
