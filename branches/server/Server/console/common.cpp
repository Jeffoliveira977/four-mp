#include <stdlib.h>

#include "common.h"

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