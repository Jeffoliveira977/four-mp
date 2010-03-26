/// \file
/// \brief Source file that contains implementation of the ConVar class.
/// \details See class description.
/// \author FaTony

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "ConVar.h"
#include "ConsoleCore.h"

ConVar::ConVar(ConsoleCore* core, const char *cvarname, const float defvalue, const char *desc, const int cvarflags, const bool hasMin, const float min, const bool hasMax, const float max)
{
	this->Init(core, cvarname, desc, cvarflags);
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
}

ConVar::ConVar(ConsoleCore* core, const char *cvarname, const int defvalue, const char *desc, const int cvarflags, const bool hasMin, const int min, const bool hasMax, const int max)
{
	this->Init(core, cvarname, desc, cvarflags);
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
}

ConVar::ConVar(ConsoleCore* core, const char *cvarname, const char *defvalue, const char *desc, const int cvarflags)
{
	this->Init(core, cvarname, desc, cvarflags);
	unsigned int length = strlen(defvalue);
	defaultvalue.type = ConVarTypeString;
	defaultvalue.value.s = (char *)calloc(length + 1, sizeof(char));
	strcpy(defaultvalue.value.s, defvalue);
	value.type = ConVarTypeString;
	value.value.s = (char *)calloc(length + 1, sizeof(char));
	strcpy(value.value.s, defvalue);
}

ConVar::~ConVar(void)
{
	concore->DeleteConVar(this->name);
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

void ConVar::Reset(void)
{
	if (value.type == ConVarTypeString)
	{
		if (defaultvalue.type == ConVarTypeString)
		{
			if (!ResizeStringBuffer(value.value.s, strlen(defaultvalue.value.s) + 1))
			{
				return;
			}
			strcpy(value.value.s, defaultvalue.value.s);
		}
		else
		{
			free(value.value.s);
		}
	}
	switch (defaultvalue.type)
	{
	case ConVarTypeFloat:
		{
			value.type = ConVarTypeFloat;
			value.value.f = defaultvalue.value.f;
			break;
		}
	case ConVarTypeInt:
		{
			value.type = ConVarTypeInt;
			value.value.i = defaultvalue.value.i;
			break;
		}
	case ConVarTypeString:
		{
			if (value.type == ConVarTypeString)
			{
				return;
			}
			value.type = ConVarTypeString;
			value.value.s = (char *)calloc(strlen(defaultvalue.value.s) + 1, sizeof(char));
			strcpy(value.value.s, defaultvalue.value.s);
			break;
		}
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

bool ConVar::SetValue(const float val)
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

bool ConVar::SetValue(const int val)
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

bool ConVar::SetValue(const char *val)
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

bool ConVar::GetBound(ConVarBoundType type, float &bound)
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

bool ConVar::GetBound(ConVarBoundType type, int &bound)
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

bool ConVar::SetBound(ConVarBoundType type, const float bound)
{
	if (defaultvalue.type != ConVarTypeFloat)
	{
		return false;
	}
	if (type == ConVarBoundLower)
	{
		if (bound > defaultvalue.value.f)
		{
			return false;
		}
		minimum.exist = true;
		minimum.type = ConVarTypeFloat;
		minimum.value.f = bound;
		if (value.type != ConVarTypeFloat)
		{
			this->Reset();
		}
		else if (value.value.f < bound)
		{
			value.value.f = bound;
		}
	}
	if (type == ConVarBoundUpper)
	{
		if (bound < defaultvalue.value.f)
		{
			return false;
		}
		maximum.exist = true;
		maximum.type = ConVarTypeFloat;
		maximum.value.f = bound;
		if (value.type != ConVarTypeFloat)
		{
			this->Reset();
		}
		else if (value.value.f > bound)
		{
			value.value.f = bound;
		}
	}
	return true;
}

bool ConVar::SetBound(ConVarBoundType type, const int bound)
{
	if (defaultvalue.type != ConVarTypeInt)
	{
		return false;
	}
	if (type == ConVarBoundLower)
	{
		if (bound > defaultvalue.value.i)
		{
			return false;
		}
		minimum.exist = true;
		minimum.type = ConVarTypeInt;
		minimum.value.i = bound;
		if (value.type != ConVarTypeInt)
		{
			this->Reset();
		}
		else if (value.value.i < bound)
		{
			value.value.i = bound;
		}
	}
	if (type == ConVarBoundUpper)
	{
		if (bound < defaultvalue.value.i)
		{
			return false;
		}
		maximum.exist = true;
		maximum.type = ConVarTypeInt;
		maximum.value.i = bound;
		if (value.type != ConVarTypeInt)
		{
			this->Reset();
		}
		else if (value.value.i > bound)
		{
			value.value.i = bound;
		}
	}
	return true;
}