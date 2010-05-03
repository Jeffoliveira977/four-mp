#pragma once

#include "ConSymbolBase.h"

class ConAlias : public ConSymbolBase
{
public:
	ConAlias(ConsoleCore *core, const char *aliasname, const char *cmdstring = "");
	~ConAlias(void);
	void SetCommandString(const char *string);
	void Execute(void);
};
