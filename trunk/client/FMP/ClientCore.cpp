#define _CRT_RAND_S
#include <stdlib.h>
#include <string.h>

#include "ClientCore.h"

ClientCore::ClientCore(void)
{
	strcpy(name, "unnamed");
	index = -1; // Should be defined in PlayerManager.h
	sessionkey = 0;
}

ClientCore::~ClientCore(void)
{
}

bool ClientCore::Load(void)
{
	rand_s(&sessionkey);
	return true;
}

bool ClientCore::IsRunning(void)
{
	return true; //TODO:
}

void ClientCore::Tick(void)
{
}

void ClientCore::Unload(void)
{
}

void ClientCore::Shutdown(void)
{
}

char *ClientCore::GetName(void)
{
	if (name == NULL)
	{
		return NULL;
	}
	char *tempstring = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(tempstring, name);
	return tempstring;
}

short ClientCore::GetIndex(void)
{
	return index;
}

bool ClientCore::SetIndex(const short i)
{
	index = i;
	return true;
}

unsigned int ClientCore::GetSessionKey(void)
{
	return sessionkey;
}