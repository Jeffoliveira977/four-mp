#pragma once

#include "ConSymbolBase.h"

enum ConVarType
{
	ConVarTypeFloat,
	ConVarTypeInt,
	ConVarTypeString
};
enum ConVarBoundType
{
	ConVarBoundLower,
	ConVarBoundUpper
};

class ConVar : public ConSymbolBase
{
public:
	ConVar(const char *cvarname, const float defvalue, const char *desc = "", const int cvarflags = 0, const bool hasMin = false, const float min = 0.0, const bool hasMax = false, const float max = 0.0);
	ConVar(const char *cvarname, const int defvalue, const char *desc = "", const int cvarflags = 0, const bool hasMin = false, const int min = 0, const bool hasMax = false, const int max = 0);
	ConVar(const char *cvarname, const char *defvalue, const char *desc = "", const int cvarflags = 0);
	~ConVar(void);
	void Reset(void);
	ConVarType GetType(void);
	ConVarType GetDefaultType(void);
	bool GetValue(float &val);
	bool GetValue(int &val);
	bool GetValue(char *&val);
	bool GetDefaultValue(float &val);
	bool GetDefaultValue(int &val);
	bool GetDefaultValue(char *&val);
	bool SetValue(const float val);
	bool SetValue(const int val);
	bool SetValue(const char *val);
	bool GetBound(ConVarBoundType type, float &bound);
	bool GetBound(ConVarBoundType type, int &bound);
	bool SetBound(ConVarBoundType type, const float bound);
	bool SetBound(ConVarBoundType type, const int bound);
private:
	union ConVarValueUnion
	{
		float f;
		int i;
		char *s;
	};
	struct ConVarValue
	{
		ConVarType type;
		ConVarValueUnion value;
	};
	struct ConVarBound
	{
		bool exist;
		ConVarType type;
		ConVarValueUnion value;
	};
	ConVarValue value;
	ConVarValue defaultvalue;
	ConVarBound minimum;
	ConVarBound maximum;
};
