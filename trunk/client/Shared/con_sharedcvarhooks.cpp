/// \file
/// \brief Source file that contains implementation of the four-mp console variable hooks.
/// \details See con_fmpcvarhooks.h.
/// \author FaTony

#include <string.h>
#include <stdlib.h>

#include "con_sharedcvarhooks.h"
#include "../Shared/ClientCore.h"

extern ClientCore client;

void ConVarHookName(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue)
{
	if (newtype == ConVarTypeString)
	{
		unsigned int length = strlen((char *)newvalue);
		if (length > 31)
		{
			strncpy(client.name, (char *)newvalue, 31);
			client.name[31] = '\0';
			return;
		}
		strncpy(client.name, (char *)newvalue, length);
		client.name[length] = '\0';
		return;
	}
	char *value;
	if (!convar->GetValue(value))
	{
		return;
	}
	unsigned int length = strlen(value);
	if (length > 31)
	{
		strncpy(client.name, value, 31);
		client.name[31] = '\0';
		return;
	}
	strncpy(client.name, value, length);
	client.name[length] = '\0';
	free(value);
}