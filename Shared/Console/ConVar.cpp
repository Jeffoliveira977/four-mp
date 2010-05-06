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
	hookbuffersize = 0;
	hookbuffer = NULL;
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
	hookbuffersize = 0;
	hookbuffer = NULL;
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
	hookbuffersize = 0;
	hookbuffer = NULL;
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
	if (hookbuffer != NULL)
	{
		free(hookbuffer);
	}
}

void ConVar::Reset(void)
{
	if (value.type == ConVarTypeString)
	{
		if (defaultvalue.type == ConVarTypeString)
		{
			if (!ResizeBuffer<char *, char, unsigned int>(value.value.s, strlen(defaultvalue.value.s) + 1))
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
	if ((value.type == ConVarTypeFloat) && (value.value.f == val))
	{
		return true;
	}
	if (((minimum.exist == true) && (minimum.type == ConVarTypeInt)) || ((maximum.exist == true) && (maximum.type == ConVarTypeInt)))
	{
		return false;
	}
	switch (value.type)
	{
	case ConVarTypeFloat:
		{
			float oldvalue = value.value.f;
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
			this->FireChangeHook(oldvalue);
			break;
		}
	case ConVarTypeInt:
		{
			int oldvalue = value.value.i;
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
			this->FireChangeHook(oldvalue);
			break;
		}
	case ConVarTypeString:
		{
			char *oldvalue = value.value.s;
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
			this->FireChangeHook(oldvalue);
			free(oldvalue);
			break;
		}
	}
	return true;
}

bool ConVar::SetValue(const int val)
{
	if ((value.type == ConVarTypeInt) && (value.value.i == val))
	{
		return true;
	}
	if (((minimum.exist == true) && (minimum.type == ConVarTypeFloat)) || ((maximum.exist == true) && (maximum.type == ConVarTypeFloat)))
	{
		return false;
	}
	switch (value.type)
	{
	case ConVarTypeFloat:
		{
			float oldvalue = value.value.f;
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
			this->FireChangeHook(oldvalue);
			break;
		}
	case ConVarTypeInt:
		{
			int oldvalue = value.value.i;
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
			this->FireChangeHook(oldvalue);
			break;
		}
	case ConVarTypeString:
		{
			char *oldvalue = value.value.s;
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
			this->FireChangeHook(oldvalue);
			free(oldvalue);
			break;
		}
	}
	return true;
}

bool ConVar::SetValue(const char *val)
{
	if ((minimum.exist == true) || (maximum.exist == true))
	{
		return false;
	}
	if ((value.type == ConVarTypeString) && (strcmp(value.value.s, val) == 0))
	{
		return true;
	}
	unsigned int length = strlen(val);
	switch (value.type)
	{
	case ConVarTypeFloat:
		{
			float oldvalue = value.value.f;
			value.value.s = (char *)calloc(length + 1, sizeof(char));
			strcpy(value.value.s, val);
			this->FireChangeHook(oldvalue);
			break;
		}
	case ConVarTypeInt:
		{
			int oldvalue = value.value.i;
			value.value.s = (char *)calloc(length + 1, sizeof(char));
			strcpy(value.value.s, val);
			this->FireChangeHook(oldvalue);
			break;
		}
	case ConVarTypeString:
		{
			char *oldvalue = (char *)calloc(strlen(value.value.s) + 1, sizeof(char));
			strcpy(oldvalue, value.value.s);
			if (!ResizeBuffer<char *, char, unsigned int>(value.value.s, length + 1))
			{
				free(oldvalue);
				return false;
			}
			strcpy(value.value.s, val);
			this->FireChangeHook(oldvalue);
			free(oldvalue);
			break;
		}
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

bool ConVar::SetBound(ConVarBoundType type, const bool set, const float bound)
{
	if (defaultvalue.type != ConVarTypeFloat)
	{
		return false;
	}
	if (type == ConVarBoundLower)
	{
		if (!set)
		{
			minimum.exist = false;
			return true;
		}
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
		if (!set)
		{
			maximum.exist = false;
			return true;
		}
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

bool ConVar::SetBound(ConVarBoundType type, const bool set, const int bound)
{
	if (defaultvalue.type != ConVarTypeInt)
	{
		return false;
	}
	if (type == ConVarBoundLower)
	{
		if (!set)
		{
			minimum.exist = false;
			return true;
		}
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
		if (!set)
		{
			maximum.exist = false;
			return true;
		}
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

bool ConVar::HookChange(void *callback)
{
	if (callback == NULL)
	{
		return false;
	}
	if (hookbuffersize == concore->maxhookspercvar)
	{
		return false;
	}
	for (unsigned char i = 0; i < hookbuffersize; i++)
	{
		if (hookbuffer[i] == callback)
		{
			return true;
		}
	}
	if (!ResizeBuffer<void **, void *, unsigned char>(hookbuffer, hookbuffersize + 1))
	{
		return false;
	}
	hookbuffer[hookbuffersize] = callback;
	hookbuffersize++;
	return true;
}

bool ConVar::UnhookChange(const void *callback)
{
	for (unsigned char i = 0; i < hookbuffersize; i++)
	{
		if (hookbuffer[i] == callback)
		{
			for (; i < (hookbuffersize - 1); i++)
			{
				hookbuffer[i] = hookbuffer[i+1];
			}
			if (!ResizeBuffer<void **, void *, unsigned char>(hookbuffer, hookbuffersize - 1))
			{
				return false;
			}
			hookbuffersize--;
			return true;
		}
	}
	return false;
}

void ConVar::FireChangeHook(const float val)
{
	if (hookbuffersize == 0)
	{
		return;
	}
	switch (value.type)
	{
	case ConVarTypeFloat:
		{
			float *oldvalue = new float;
			*oldvalue = val;
			float *newvalue = new float;
			*newvalue = value.value.f;
			for (unsigned char i = 0; i < hookbuffersize; i++)
			{
				((void (*) (const ConVar *, const ConVarType, void *, const ConVarType, void *))hookbuffer[i])(this, ConVarTypeFloat, oldvalue, ConVarTypeFloat, newvalue);
			}
			free(oldvalue);
			free(newvalue);
			break;
		}
	case ConVarTypeInt:
		{
			float *oldvalue = new float;
			*oldvalue = val;
			int *newvalue = new int;
			*newvalue = value.value.i;
			for (unsigned char i = 0; i < hookbuffersize; i++)
			{
				((void (*) (const ConVar *, const ConVarType, void *, const ConVarType, void *))hookbuffer[i])(this, ConVarTypeFloat, oldvalue, ConVarTypeInt, newvalue);
			}
			free(oldvalue);
			free(newvalue);
			break;
		}
	case ConVarTypeString:
		{
			float *oldvalue = new float;
			*oldvalue = val;
			char *newvalue = (char *)calloc(strlen(value.value.s) + 1, sizeof(char));
			strcpy(newvalue, value.value.s);
			for (unsigned char i = 0; i < hookbuffersize; i++)
			{
				((void (*) (const ConVar *, const ConVarType, void *, const ConVarType, void *))hookbuffer[i])(this, ConVarTypeFloat, oldvalue, ConVarTypeString, newvalue);
			}
			free(oldvalue);
			free(newvalue);
			break;
		}
	}
}

void ConVar::FireChangeHook(const int val)
{
	if (hookbuffersize == 0)
	{
		return;
	}
	switch (value.type)
	{
	case ConVarTypeFloat:
		{
			int *oldvalue = new int;
			*oldvalue = val;
			float *newvalue = new float;
			*newvalue = value.value.f;
			for (unsigned char i = 0; i < hookbuffersize; i++)
			{
				((void (*) (const ConVar *, const ConVarType, void *, const ConVarType, void *))hookbuffer[i])(this, ConVarTypeInt, oldvalue, ConVarTypeFloat, newvalue);
			}
			free(oldvalue);
			free(newvalue);
			break;
		}
	case ConVarTypeInt:
		{
			int *oldvalue = new int;
			*oldvalue = val;
			int *newvalue = new int;
			*newvalue = value.value.i;
			for (unsigned char i = 0; i < hookbuffersize; i++)
			{
				((void (*) (const ConVar *, const ConVarType, void *, const ConVarType, void *))hookbuffer[i])(this, ConVarTypeInt, oldvalue, ConVarTypeInt, newvalue);
			}
			free(oldvalue);
			free(newvalue);
			break;
		}
	case ConVarTypeString:
		{
			int *oldvalue = new int;
			*oldvalue = val;
			char *newvalue = (char *)calloc(strlen(value.value.s) + 1, sizeof(char));
			strcpy(newvalue, value.value.s);
			for (unsigned char i = 0; i < hookbuffersize; i++)
			{
				((void (*) (const ConVar *, const ConVarType, void *, const ConVarType, void *))hookbuffer[i])(this, ConVarTypeInt, oldvalue, ConVarTypeString, newvalue);
			}
			free(oldvalue);
			free(newvalue);
			break;
		}
	}
}

void ConVar::FireChangeHook(const char *val)
{
	if (hookbuffersize == 0)
	{
		return;
	}
	switch (value.type)
	{
	case ConVarTypeFloat:
		{
			char *oldvalue = (char *)calloc(strlen(val) + 1, sizeof(char));
			strcpy(oldvalue, val);
			float *newvalue = new float;
			*newvalue = value.value.f;
			for (unsigned char i = 0; i < hookbuffersize; i++)
			{
				((void (*) (const ConVar *, const ConVarType, void *, const ConVarType, void *))hookbuffer[i])(this, ConVarTypeString, oldvalue, ConVarTypeFloat, newvalue);
			}
			free(oldvalue);
			free(newvalue);
			break;
		}
	case ConVarTypeInt:
		{
			char *oldvalue = (char *)calloc(strlen(val) + 1, sizeof(char));
			strcpy(oldvalue, val);
			int *newvalue = new int;
			*newvalue = value.value.i;
			for (unsigned char i = 0; i < hookbuffersize; i++)
			{
				((void (*) (const ConVar *, const ConVarType, void *, const ConVarType, void *))hookbuffer[i])(this, ConVarTypeString, oldvalue, ConVarTypeInt, newvalue);
			}
			free(oldvalue);
			free(newvalue);
			break;
		}
	case ConVarTypeString:
		{
			char *oldvalue = (char *)calloc(strlen(val) + 1, sizeof(char));
			strcpy(oldvalue, val);
			char *newvalue = (char *)calloc(strlen(value.value.s) + 1, sizeof(char));
			strcpy(newvalue, value.value.s);
			for (unsigned char i = 0; i < hookbuffersize; i++)
			{
				((void (*) (const ConVar *, const ConVarType, void *, const ConVarType, void *))hookbuffer[i])(this, ConVarTypeString, oldvalue, ConVarTypeString, newvalue);
			}
			free(oldvalue);
			free(newvalue);
			break;
		}
	}
}