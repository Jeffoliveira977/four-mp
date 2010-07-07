#include <windows.h>
#include "../../../Shared/logging/log.h"
#include "hook.h"
#include "natives.h"

extern DWORD dwLoadOffset;

int GetNativeAddressByHash(unsigned int a1)
{
	if(a1 == -1) return 0;
	//Log::Debug("# GetNativeAddress: 0x%x , natives count: %d",a1,*(DWORD*)(0x18DF6EC+dwLoadOffset));

	DWORD pointer1 = *(DWORD*)SCRIPT_POINTER_1;
	DWORD pointer2 = *(DWORD*)SCRIPT_POINTER_2;

	//Log::Debug("# pointers: [0x%x]:(%d),[0x%x]:(%d)",pointer1,pointer1,pointer2,pointer2);

	unsigned int v2;
	unsigned int v3;
	unsigned int v4;
	unsigned int hash_pointer;

	if(!pointer1) 
	{
		Log::Debug(L"# not find pointer");
		return 0;
	}
	// reset data
	v3 = a1 % pointer1;
	v4 = a1;
	// first pointer
	hash_pointer = *(DWORD *)(pointer2 + 8 * v3);
	while(hash_pointer != a1)
	{
		v4 = (v4 >> 1) + 1;
		v3 = (v4 + v3) % pointer1;
		hash_pointer = *(DWORD *)(pointer2 + (8 * v3));
		//Log::Debug("# hash_pointer: 0x%x [%d]",hash_pointer,hash_pointer);
	}
	//Log::Debug("# hash_pointer: 0x%x [%d]",hash_pointer,hash_pointer);
	if(hash_pointer == 0)
	{
		Log::Debug(L"# hash_pointer null");
		return 0;
	}
	int addr = (int)(*(DWORD*)(pointer2 + (8 * v3) + 4));
	//Log::Debug("# addr = 0x%x",addr);
	return addr;
}
