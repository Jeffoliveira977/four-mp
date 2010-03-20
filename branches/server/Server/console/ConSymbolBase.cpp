#include <stdlib.h>
#include <string.h>

#include "ConSymbolBase.h"

ConSymbolBase::ConSymbolBase(void)
{
}

ConSymbolBase::~ConSymbolBase(void)
{
}

char *ConSymbolBase::GetName(void)
{
	char *tempname = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(tempname, name);
	return tempname;
}

char *ConSymbolBase::GetDescription(void)
{
	char *tempdescription = (char *)calloc(strlen(description) + 1, sizeof(char));
	strcpy(tempdescription, description);
	return tempdescription;
}

int ConSymbolBase::GetFlags(void)
{
	return flags;
}

void ConSymbolBase::SetFlags(const int f)
{
	flags = f;
}

void ConSymbolBase::Init(const char *symbolname, const char *desc, const int symbolflags)
{
	name = (char *)calloc(strlen(symbolname) + 1, sizeof(char));
	strcpy(name, symbolname);
	description = (char *)calloc(strlen(desc) + 1, sizeof(char));
	strcpy(description, desc);
	flags = symbolflags;
}

void ConSymbolBase::Uninit(void)
{
	free(name);
	free(description);
}
