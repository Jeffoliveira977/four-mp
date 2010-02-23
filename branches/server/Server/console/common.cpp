#include <stdlib.h>

#include "common.h"

bool ResizeArrayBuffer(char **&buffer, const unsigned short size)
{
	char **tempbuffer = (char **)realloc(*&buffer, size * sizeof(char *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

bool ResizeStringBuffer(char *&buffer, const unsigned int size)
{
	char *tempbuffer = (char *)realloc(buffer, size * sizeof(char));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}