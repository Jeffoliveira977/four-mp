/// \file
/// \brief Source file that contains implementation of the ConAlias class.
/// \details See class description.
/// \author FaTony

#include <string.h>

#include "ConAlias.h"
#include "common.h"
#include "ConsoleCore.h"

ConAlias::ConAlias(ConsoleCore *core, const char *aliasname, const char *cmdstring)
{
	this->Init(core, aliasname, cmdstring);
}

ConAlias::~ConAlias(void)
{
	this->Uninit();
}

void ConAlias::SetCommandString(const char *string)
{
	ResizeBuffer<char *, char, unsigned int>(description, strlen(string) + 1);
	strcpy(description, string);
}

void ConAlias::Execute(void)
{
	concore->InterpretLine(description);
}