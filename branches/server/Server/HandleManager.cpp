#include <stdlib.h>

#include "main.h"
#include "HandleManager.h"
#include "CoreHandleTypesManager.h"
#include "PluginManager.h"
#include "VirtualMachineManager.h"

extern CoreHandleTypesManager chtm;
extern PluginManager pm;
extern VirtualMachineManager vmm;

HandleManager::HandleManager(void)
{
	maxtypebuffersize = 65535;
	maxhandlesperowner = 16384;
	pluginowneroffset = 2 + vmm.GetMaxFilterScripts();
	maxcountbuffersize =  pluginowneroffset + pm.GetMaxPluginBufferSize();
	maxhandlebuffersize = maxcountbuffersize * maxhandlesperowner;
	typebuffer = (HandleType **)calloc(NUM_CORE_HANDLE_TYPES, sizeof(HandleType *));
	for (unsigned short i = 0; i < NUM_CORE_HANDLE_TYPES; i++)
	{
		typebuffer[i] = new HandleType;
		typebuffer[i]->owner = 255;
	}
	typebuffersize = NUM_CORE_HANDLE_TYPES;
	handlebuffersize = 0;
	countbuffersize = 0;
}

HandleManager::~HandleManager(void)
{
	for (unsigned short i = 0; i < typebuffersize; i++)
	{
		delete typebuffer[i];
	}
	delete typebuffer;
	for (int i = 0; i < handlebuffersize; i++)
	{
		delete handlebuffer[i];
	}
	delete handlebuffer;
	for (short i = 0; i < countbuffersize; i++)
	{
		delete countbuffer[i];
	}
	delete countbuffer;
}

void HandleManager::ReleaseAllHandleTypes(const unsigned char pluginindex)
{
	for (unsigned short index = 0; index < typebuffersize; index++)
	{
		this->ReleaseHandleType(pluginindex, index);
	}
}

bool HandleManager::RequestNewHandleType(const unsigned char pluginindex, unsigned short &typeindex)
{
	if (!this->GetHandleTypeFreeSlot(typeindex))
	{
		return false;
	}
	if (typeindex == typebuffersize)
	{
		if (!this->ResizeHandleTypeBuffer(typebuffer, typebuffersize + 1))
		{
			return false;
		}
		typebuffersize++;
	}
	typebuffer[typeindex] = new HandleType;
	typebuffer[typeindex]->owner = pluginindex;
	return true;
}

bool HandleManager::ReleaseHandleType(const unsigned char pluginindex, const unsigned short typeindex)
{
	if (typeindex >= typebuffersize)
	{
		return false;
	}
	if (typebuffer[typeindex] == NULL)
	{
		return false;
	}
	if (typebuffer[typeindex]->owner != pluginindex)
	{
		return false;
	}
	delete typebuffer[typeindex];
	typebuffer[typeindex] = NULL;
	return true;
}

void HandleManager::CloseAllHandles(const short owner)
{
	if ((owner < 0) || (owner >= countbuffersize))
	{
		return;
	}
	for (int i = 0; i < handlebuffersize; i++)
	{
		if (handlebuffer[i] != NULL)
		{
			for (short j = 0; (handlebuffer[i] != NULL) && (j < handlebuffer[i]->numowners); j++)
			{
				if (handlebuffer[i]->owner[j] == owner)
				{
					this->CloseHandle(owner, i);
				}
			}
		}
	}
}

int HandleManager::AddNewHandle(const short owner, const unsigned short type, void *ptr)
{
	if (ptr == NULL)
	{
		return INVALID_HANDLE;
	}
	if ((owner < 0) || (owner >= maxcountbuffersize))
	{
		return false;
	}
	int index = this->FindHandle(ptr);
	if (index != INVALID_HANDLE)
	{
		if (handlebuffer[index]->type != type)
		{
			return INVALID_HANDLE;
		}
		if(!this->AddHandleOwner(index, owner))
		{
			return INVALID_HANDLE;
		}
		return index;

	}
	index = this->GetHandleFreeSlot();
	if (index == INVALID_HANDLE)
	{
		return INVALID_HANDLE;
	}
	if (!this->IncreaseHandleCount(owner))
	{
		return INVALID_HANDLE;
	}
	if (index == handlebuffersize)
	{
		if (!this->ResizeHandleBuffer(handlebuffer, index + 1))
		{
			return INVALID_HANDLE;
		}
		handlebuffersize = index + 1;
	}
	handlebuffer[index] = new Handle;
	handlebuffer[index]->numowners = 1;
	handlebuffer[index]->owner = new short;
	handlebuffer[index]->owner[0] = owner;
	handlebuffer[index]->type = type;
	handlebuffer[index]->ptr = ptr;
	return index;
}

bool HandleManager::GetHandleType(const short owner, const int index, unsigned short &type)
{
	if ((owner < 0) || (owner >= countbuffersize))
	{
		return false;
	}
	if ((index < 0) || (index >= handlebuffersize))
	{
		return false;
	}
	if (handlebuffer[index] == NULL)
	{
		return false;
	}
	if (!this->IsHandleOwned(index, owner))
	{
		return false;
	}
	type = handlebuffer[index]->type;
	return true;
}

void *HandleManager::GetHandlePointer(const short owner, const int index)
{
	if ((owner < 0) || (owner >= countbuffersize))
	{
		return NULL;
	}
	if ((index < 0) || (index >= handlebuffersize))
	{
		return NULL;
	}
	if (handlebuffer[index] == NULL)
	{
		return NULL;
	}
	if (!this->IsHandleOwned(index, owner))
	{
		return NULL;
	}
	return handlebuffer[index]->ptr;
}

bool HandleManager::SetHandlePointer(const short owner, const int index, void *ptr)
{
	if ((owner < 0) || (owner >= countbuffersize))
	{
		return false;
	}
	if ((index < 0) || (index >= handlebuffersize))
	{
		return false;
	}
	if (handlebuffer[index] == NULL)
	{
		return false;
	}
	if (!this->IsHandleOwned(index, owner))
	{
		return false;
	}
	handlebuffer[index]->ptr = ptr;
	return true;
}

bool HandleManager::CloseHandle(const short owner, const int index)
{
	if ((owner < 0) || (owner >= countbuffersize))
	{
		return false;
	}
	if ((index < 0) || (index >= handlebuffersize))
	{
		return false;
	}
	if (handlebuffer[index] == NULL)
	{
		return false;
	}
	if (!this->DeleteHandleOwner(index, owner))
	{
		return false;
	}
	return true;
}

bool HandleManager::GetHandleTypeFreeSlot(unsigned short &index)
{
	for (index = 0; index < typebuffersize; index++)
	{
		if (typebuffer[index] == NULL)
		{
			return true;
		}
	}
	if (typebuffersize == maxtypebuffersize)
	{
		return false;
	}
	return true;
}

bool HandleManager::ResizeHandleTypeBuffer(HandleType **&buffer, const unsigned short size)
{
	HandleType **tempbuffer = (HandleType **)realloc(*&buffer, size * sizeof(HandleType *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

int HandleManager::GetHandleFreeSlot(void)
{
	int index;
	for (index = 0; index < handlebuffersize; index++)
	{
		if (handlebuffer[index] == NULL)
		{
			return index;
		}
	}
	if (handlebuffersize == maxhandlebuffersize)
	{
		return INVALID_HANDLE;
	}
	return index;
}

int HandleManager::FindHandle(const void *ptr)
{
	for (int index = 0; index < handlebuffersize; index++)
	{
		if ((handlebuffer[index] != NULL) && (handlebuffer[index]->ptr == ptr))
		{
			return index;
		}
	}
	return INVALID_HANDLE;
}

bool HandleManager::IsHandleOwned(const int index, const short owner)
{
	if ((index < 0) || (index >= handlebuffersize))
	{
		return false;
	}
	if ((owner < 0) || (owner >= countbuffersize))
	{
		return false;
	}
	if (handlebuffer[index] == NULL)
	{
		return false;
	}
	for (short i = 0; i < handlebuffer[index]->numowners; i++)
	{
		if (handlebuffer[index]->owner[i] == owner)
		{
			return true;
		}
	}
	return false;
}

bool HandleManager::AddHandleOwner(const int index, const short owner)
{
	if ((index < 0) || (index >= handlebuffersize))
	{
		return false;
	}
	if ((owner < 0) || (owner >= maxcountbuffersize))
	{
		return false;
	}
	if (handlebuffer[index] == NULL)
	{
		return false;
	}
	if (this->IsHandleOwned(index, owner))
	{
		return true;
	}
	if (!this->ResizeHandleOwnerBuffer(handlebuffer[index]->owner, handlebuffer[index]->numowners + 1))
	{
		return false;
	}
	handlebuffer[index]->owner[handlebuffer[index]->numowners] = owner;
	handlebuffer[index]->numowners++;
	if (!this->IncreaseHandleCount(owner))
	{
		return false;
	}
	return true;
}

bool HandleManager::DeleteHandleOwner(const int index, const short owner)
{
	if ((index < 0) || (index >= handlebuffersize))
	{
		return false;
	}
	if ((owner < 0) || (owner >= countbuffersize))
	{
		return false;
	}
	if (handlebuffer[index] == NULL)
	{
		return false;
	}
	short i = 0;
	while ((handlebuffer[index]->owner[i] != owner) && (i < handlebuffer[index]->numowners))
	{
		i++;
	}
	if (i == handlebuffer[index]->numowners)
	{
		return false;
	}
	for (;i < (handlebuffer[index]->numowners - 1); i++)
	{
		handlebuffer[index]->owner[i] = handlebuffer[index]->owner[i+1];
	}
	if (!this->ResizeHandleOwnerBuffer(handlebuffer[index]->owner, handlebuffer[index]->numowners - 1))
	{
		return false;
	}
	handlebuffer[index]->numowners--;
	if (handlebuffer[index]->numowners == 0)
	{
		if (handlebuffer[index]->type < NUM_CORE_HANDLE_TYPES)
		{
			chtm.CloseHandle(index);
		}
		else
		{
			//TODO: Dynamic handle types
			free(handlebuffer[index]->ptr);
		}
		free(handlebuffer[index]);
		handlebuffer[index] = NULL;
	}
	this->DecreaseHandleCount(owner);
	return true;
}

bool HandleManager::ResizeHandleOwnerBuffer(short *&buffer, const short size)
{
	short *tempbuffer = (short *)realloc(buffer, size * sizeof(short));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

bool HandleManager::ResizeHandleBuffer(Handle **&buffer, const int size)
{
	Handle **tempbuffer = (Handle **)realloc(*&buffer, size * sizeof(Handle *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}

bool HandleManager::IncreaseHandleCount(const short owner)
{
	if ((owner < 0) || (owner >= maxcountbuffersize))
	{
		return false;
	}
	if (owner >= countbuffersize)
	{
		if (!this->ResizeHandleCountBuffer(countbuffer, owner + 1))
		{
			return false;
		}
		for (short i = countbuffersize; i <= owner; i++)
		{
			countbuffer[i] = NULL;
		}
		countbuffersize = owner + 1;
	}
	if (countbuffer[owner] == NULL)
	{
		countbuffer[owner] = new short;
		countbuffer[owner][0] = 1;
		return true;
	}
	if (countbuffer[owner][0] == maxhandlesperowner)
	{
		print("MEMORY LEAK IN MODULE %d. Unloading.", owner);
		this->CloseAllHandles(owner);
		return false;
	}
	countbuffer[owner][0]++;
	return true;
}

bool HandleManager::DecreaseHandleCount(const short owner)
{
	if ((owner < 0) || (owner >= countbuffersize))
	{
		return false;
	}
	if (countbuffer[owner] == NULL)
	{
		return false;
	}
	if (countbuffer[owner] <= 0)
	{
		return false;
	}
	countbuffer[owner]--;
	return true;
}

bool HandleManager::ResizeHandleCountBuffer(short **&buffer, const short size)
{
	short **tempbuffer = (short **)realloc(*&buffer, size * sizeof(short *));
	if ((tempbuffer == NULL) && (size != 0))
	{
		return false;
	}
	buffer = tempbuffer;
	return true;
}