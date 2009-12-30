#include <windows.h>
#include "log.h"
#include "natives.h"
extern DWORD dwLoadOffset;

ptr FNA(char *n)
{
	DWORD signature = *(DWORD *)(0x608C34+dwLoadOffset);

	DWORD addrs[2] = { 0x0, 0x0 };

	if (signature == 0x831F7518) {  }
	else if (signature == 0xC483FFE4) {  }
	else if (signature == 0x280F0000) {  }
	else if (signature == 0x110FF300) { addrs[0] = 0x61A1D0; addrs[1] = 0x61A100; }
	else if (signature == 0x108b1874) { }


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

