#include <windows.h>
#include "log.h"
#include "natives.h"
extern DWORD dwLoadOffset;

ptr FNA(char *n)
{
	DWORD addrs[2] = { 0x0, 0x0 };
	addrs[0] = 0x6CF530; 
	addrs[1] = 0x6CC670; 

	ptr fa = ptr_cast( dwLoadOffset +  addrs[0]);
	int hash;
	_asm
	{
		mov eax, n;
		call fa;
		mov hash, eax;
	}
    fa = ptr_cast( dwLoadOffset +  addrs[1]);
    ptr na;
    _asm
    {
        push esi;
        mov esi, hash;
        call fa;
        pop esi;
        mov na, eax;
    }
    return na;
}

