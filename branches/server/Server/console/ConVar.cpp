#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "ConVar.h"
#include "ConsoleCore.h"

extern ConsoleCore concore;

ConVar::ConVar(const char *cvarname, const float defvalue, const char *desc, const int cvarflags, const bool hasMin, const float min, const bool hasMax, const float max)
{
	this->Init(cvarname, desc, cvarflags);
	defaultvalue.type = ConVarTypeFloat;
	defaultvalue.value.f = defvalue;
	value = defaultvalue;
	if (hasMin)
	{
		minimum.exist = true;
		minimum.type = ConVarTypeFloat;
		minimum.value.f = min;
	}
	else
	{
		minimum.exist = false;
	}
	if (hasMax)
	{
		maximum.exist = true;
		maximum.type = ConVarTypeFloat;
		maximum.value.f = max;
	}
	else
	{
		maximum.exist = false;
	}
	this->RegisterConVar(cvarname);
}

ConVar::ConVar(const char *cvarname, const int defvalue, const char *desc, const int cvarflags, const bool hasMin, const int min, const bool hasMax, const int max)
{
	this->Init(cvarname, desc, cvarflags);
	defaultvalue.type = ConVarTypeInt;
	defaultvalue.value.i = defvalue;
	value = defaultvalue;
	if (hasMin)
	{
		minimum.exist = true;
		minimum.type = ConVarTypeInt;
		minimum.value.i = min;
	}
	else
	{
		minimum.exist = false;
	}
	if (hasMax)
	{
		maximum.exist = true;
		maximum.type = ConVarTypeInt;
		maximum.value.i = max;
	}
	else
	{
		maximum.exist = false;
	}
	this->RegisterConVar(cvarname);
}

ConVar::ConVar(const char *cvarname, const char *defvalue, const char *desc, const int cvarflags)
{
	this->Init(cvarname, desc, cvarflags);
	defaultvalue.type = ConVarTypeString;
	unsigned int length = strlen(desc);
	defaultvalue.value.s = (char *)calloc(length + 1, sizeof(char));
	strcpy(defaultvalue.value.s, defvalue);
	value = defaultvalue;
	this->RegisterConVar(cvarname);
}

ConVar::~ConVar(void)
{
	this->Uninit();
	if (defaultvalue.type == ConVarTypeString)
	{
		free(defaultvalue.value.s);
	}
	if (value.type == ConVarTypeString)
	{
		free(value.value.s);
	}
}

ConVarType ConVar::GetType(void)
{
	return value.type;
}

ConVarType ConVar::GetDefaultType(void)
{
	return defaultvalue.type;
}

bool ConVar::GetValue(float &val)
{
	if (value.type != ConVarTypeFloat)
	{
		return false;
	}
	val = value.value.f;
	return true;
}

bool ConVar::GetValue(int &val)
{
	if (value.type != ConVarTypeInt)
	{
		return false;
	}
	val = value.value.i;
	return true;
}

bool ConVar::GetValue(char *&val)
{
	if (value.type != ConVarTypeString)
	{
		switch (value.type)
		{
		case ConVarTypeFloat:
			{
				val = (char *)calloc(_scprintf("%f", value.value.f) + 1, sizeof(char));
				sprintf(val, "%f", value.value.f);
				return true;
				break;
			}
		case ConVarTypeInt:
			{
				val = (char *)calloc(_scprintf("%d", value.value.i) + 1, sizeof(char));
				sprintf(val, "%d", value.value.i);
				return true;
				break;
			}
		}
		return false;
	}
	val = (char *)calloc(strlen(value.value.s) + 1, sizeof(char));
	strcpy(val, value.value.s);
	return true;
}

bool ConVar::GetDefaultValue(float &val)
{
	if (defaultvalue.type != ConVarTypeFloat)
	{
		return false;
	}
	val = defaultvalue.value.f;
	return true;
}

bool ConVar::GetDefaultValue(int &val)
{
	if (defaultvalue.type != ConVarTypeInt)
	{
		return false;
	}
	val = defaultvalue.value.i;
	return true;
}

bool ConVar::GetDefaultValue(char *&val)
{
	if (defaultvalue.type != ConVarTypeString)
	{
		switch (defaultvalue.type)
		{
		case ConVarTypeFloat:
			{
				val = (char *)calloc(_scprintf("%f", defaultvalue.value.f) + 1, sizeof(char));
				sprintf(val, "%f", defaultvalue.value.f);
				return true;
				break;
			}
		case ConVarTypeInt:
			{
				val = (char *)calloc(_scprintf("%d", defaultvalue.value.i) + 1, sizeof(char));
				sprintf(val, "%d", defaultvalue.value.i);
				return true;
				break;
			}
		}
		return false;
	}
	val = (char *)calloc(strlen(defaultvalue.value.s) + 1, sizeof(char));
	strcpy(val, defaultvalue.value.s);
	return true;
}

bool ConVar::SetValue(float val)
{
	if (((minimum.exist == true) && (minimum.type == ConVarTypeInt)) || ((maximum.exist == true) && (maximum.type == ConVarTypeInt)))
	{
		return false;
	}
	if (value.type == ConVarTypeString)
	{
		free(value.value.s);
	}
	bool set = false;
	if ((minimum.exist) && (val < minimum.value.f))
	{
		value.value.f = minimum.value.f;
		set = true;
	}
	if ((maximum.exist) && (val > maximum.value.f))
	{
		value.value.f = maximum.value.f;
		set = true;
	}
	if (!set)
	{
		value.value.f = val;
	}
	return true;
}

bool ConVar::SetValue(int val)
{
	if (((minimum.exist == true) && (minimum.type == ConVarTypeFloat)) || ((maximum.exist == true) && (maximum.type == ConVarTypeFloat)))
	{
		return false;
	}
	if (value.type == ConVarTypeString)
	{
		free(value.value.s);
	}
	bool set = false;
	if ((minimum.exist) && (val < minimum.value.i))
	{
		value.value.i = minimum.value.i;
		set = true;
	}
	if ((maximum.exist) && (val > maximum.value.i))
	{
		value.value.i = maximum.value.i;
		set = true;
	}
	if (!set)
	{
		value.value.i = val;
	}
	return true;
}

bool ConVar::SetValue(char *val)
{
	if ((minimum.exist == true) || (maximum.exist == true))
	{
		return false;
	}
	unsigned int length = strlen(val);
	if (value.type == ConVarTypeString)
	{
		if (!ResizeStringBuffer(value.value.s, length + 1))
		{
			return false;
		}
		strcpy(value.value.s, val);
	}
	else
	{
		value.value.s = (char *)calloc(length + 1, sizeof(char));
		strcpy(value.value.s, val);
	}
	return true;
}

bool ConVar::GetBoundFloat(ConVarBoundType type, float &bound)
{
	ConVarBound tempbound;
	if (type == ConVarBoundLower)
	{
		tempbound = minimum;
	}
	else
	{
		tempbound = maximum;
	}
	if ((tempbound.exist == false) || (tempbound.type != ConVarTypeFloat))
	{
		return false;
	}
	bound = tempbound.value.f;
	return true;
}

bool ConVar::GetBoundInt(ConVarBoundType type, int &bound)
{
	ConVarBound tempbound;
	if (type == ConVarBoundLower)
	{
		tempbound = minimum;
	}
	else
	{
		tempbound = maximum;
	}
	if ((tempbound.exist == false) || (tempbound.type != ConVarTypeInt))
	{
		return false;
	}
	bound = tempbound.value.i;
	return true;
}

void ConVar::RegisterConVar(const char *cvarname)
{
	ConsoleSymbol *tempsymbol = (ConsoleSymbol *)calloc(1, sizeof(ConsoleSymbol));
	tempsymbol->name = (char *)calloc(strlen(cvarname) + 1, sizeof(char));
	strcpy(tempsymbol->name, cvarname);
	tempsymbol->type = ConsoleSymbolTypeConVar;
	tempsymbol->ptr.convar = this;
	concore.AddConsoleSymbol(tempsymbol);
}
