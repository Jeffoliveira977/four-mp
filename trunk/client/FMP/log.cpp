#include <stdarg.h>
#include <windows.h>
#include <vector>

#include "log.h"

namespace Log
{
	std::vector<Logger*> logging;
	bool b_Debug = 1;
	bool b_Info = 1;
	bool b_Error = 1;
	bool b_Warning = 1;
	bool b_Logging = 1;

	void SetState(unsigned int state)
	{
		b_Debug = 0;
		b_Info = 0;
		b_Error = 0;
		b_Warning = 0;

		while(state > 0)
		{
			if(state >= LOG_WARNING)
			{
				b_Warning = 1;
				state -= LOG_WARNING;
			}
			if(state >= LOG_ERROR)
			{
				b_Error = 1;
				state -= LOG_ERROR;
			}
			if(state >= LOG_INFO)
			{
				b_Info = 1;
				state -= LOG_INFO;
			}
			if(state >= LOG_DEBUG)
			{
				b_Debug = 1;
				state -= LOG_DEBUG;
			}
			if(state >= LOG_ON)
			{
				b_Logging = 1;
				state -= LOG_ON;
			}
		}

		b_Logging = b_Logging || b_Debug || b_Info || b_Error || b_Warning;
	}

	void AddLogger(Logger* iLog)
	{
		logging.push_back(iLog);
	}

	void Log::Debug(const char *string, ...)
	{
		if(logging.size() == 0 || !b_Logging || !b_Debug) return;

		va_list arglist; 
		va_start(arglist, string); 
		for(int i = 0; i < (int)logging.size(); i++)
			logging[i]->Log("Debug", string, arglist);		
		va_end(arglist); 
	}
	void Info(const char *string, ...)
	{
		if(logging.size() == 0 || !b_Logging || !b_Info) return;

		va_list arglist; 
		va_start(arglist, string); 
		for(int i = 0; i < (int)logging.size(); i++)
			logging[i]->Log("Info", string, arglist);		
		va_end(arglist); 
	}
	void Error(const char *string, ...)
	{
		if(logging.size() == 0 || !b_Logging || !b_Error) return;

		va_list arglist; 
		va_start(arglist, string); 
		for(int i = 0; i < (int)logging.size(); i++)
			logging[i]->Log("Error", string, arglist);		
		va_end(arglist); 
	}
	void Warning(const char *string, ...)
	{
		if(logging.size() == 0 || !b_Logging || !b_Warning) return;

		va_list arglist; 
		va_start(arglist, string); 
		for(int i = 0; i < (int)logging.size(); i++)
			logging[i]->Log("Warning", string, arglist);		
		va_end(arglist); 
	}
	void Other(const char *type, const char *string, ...)
	{
		if(logging.size() == 0 || !b_Logging) return;

		va_list arglist; 
		va_start(arglist, string); 
		for(int i = 0; i < (int)logging.size(); i++)
			logging[i]->Log(type, string, arglist);		
		va_end(arglist); 
	}
	void Void(const char *string, ...)
	{
		if(logging.size() == 0 || !b_Logging) return;

		va_list arglist; 
		va_start(arglist, string); 
		for(int i = 0; i < (int)logging.size(); i++)
			logging[i]->Log("", string, arglist);		
		va_end(arglist); 
	}
};
