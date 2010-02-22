#pragma once

#include "ConSymbolBase.h"

class ConCmd : public ConSymbolBase
{
public:
	ConCmd(const char *cmdname, void *callback, const char *desc = "", const int cmdflags = 0);
	~ConCmd(void);
	void Execute(void);
private:
	void *handler;
};
