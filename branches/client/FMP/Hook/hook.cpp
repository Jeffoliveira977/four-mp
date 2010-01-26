#include "classes.h"
#include "hook.h"
#include "../log.h"

FMPHook::FMPHook()
{
	debug("FMPHook::FMPHook called");
	debug("FMPHook::FMPHook complete");
}

FMPHook::~FMPHook()
{
	debug("FMPHook::~FMPHook called");
	debug("FMPHook::~FMPHook complete");
}

ThreadStates FMPHook::reset(unsigned int hash,int v2,int i3)
{
	debug("FMPHook::reset called");
	debug("FMPHook::reset complete");
	return FMPThread::reset(hash,v2,i3);
}

void FMPHook::FiberStart(void* parameter)
{
	debug("FMPHook::FiberStart called");
	FMPHook* Thread = ptr_cast<FMPHook>(parameter);
	debug("FMPHook::FiberStart call GameThread");
	Thread->GameThread();
	Thread->m_bKillRequested = true;
	debug("FMPHook::FiberStart Swith");
	SwitchToFiber(Thread->m_pPrimaryFiber);
	debug("FMPHook::FiberStart complete");
}

ThreadStates FMPHook::run(int i1)
{
	debug("FMPHook::run called");

	if(m_pPrimaryFiber == NULL)
	{
		
		if (!IsBadReadPtr(GetCurrentFiber(),sizeof(ptr)))
		{
			m_pPrimaryFiber = GetCurrentFiber();
		}
		else
		{
			m_pPrimaryFiber = ConvertThreadToFiber(NULL);
		}

		m_pScriptFiber = CreateFiber(0,&FiberStart,this);
	}

	scrThread* old = GetActiveThread();
	SetActiveThread(this);

	if(GetTickCount() > m_nWaitTick) SwitchToFiber(m_pScriptFiber);

	SetActiveThread(old);
	debug("FMPHook::run complete");
	return m_context.eThreadState;
}

void FMPHook::wait(unsigned int timeMS)
{
	debug("FMPHook::wait called");
	m_nWaitTick = GetTickCount() + timeMS;
	SwitchToFiber(m_pPrimaryFiber);
	debug("FMPHook::wait complete");
}

bool FMPHook::IsThreadAlive()
{
	return m_context.eThreadState != ThreadStateKilled;
}

//-----------------------------------------------------------

void InstallMethodHook(DWORD dwInstallAddress,DWORD dwHookFunction)
{
	DWORD oldProt, oldProt2;
	VirtualProtect((LPVOID)dwInstallAddress,4,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PDWORD)dwInstallAddress = (DWORD)dwHookFunction;
	VirtualProtect((LPVOID)dwInstallAddress,4,oldProt,&oldProt2);
}

//-----------------------------------------------------------

void InstallHook(DWORD dwInstallAddress,DWORD dwHookFunction,DWORD dwHookStorage,BYTE* pbyteJmpCode,int iJmpCodeSize)
{
	DWORD oldProt, oldProt2;

	// Install the pointer to procaddr.
	VirtualProtect((PVOID)dwHookStorage,4,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PDWORD)dwHookStorage = (DWORD)dwHookFunction;
	VirtualProtect((PVOID)dwHookStorage,4,oldProt,&oldProt2);

	// Install the Jmp code.
	VirtualProtect((PVOID)dwInstallAddress,iJmpCodeSize,PAGE_EXECUTE_READWRITE,&oldProt);		
	memcpy((PVOID)dwInstallAddress,pbyteJmpCode,iJmpCodeSize);
	VirtualProtect((PVOID)dwInstallAddress,iJmpCodeSize,oldProt,&oldProt2);	
}

//-----------------------------------------------------------

bool Unlock(void *address, int len)
{
	DWORD oldp;
		// Shut up the warnings :D
	return !!VirtualProtect(address, len, PAGE_EXECUTE_READWRITE, &oldp);
}

//-----------------------------------------------------------

void Call(DWORD from, DWORD to)
{
	if (Unlock((LPVOID)from, 5))
	{
		DWORD disp = to - (from + 5);
		*(BYTE *)(from) = 0xE8;
		*(DWORD *)(from + 1) = (DWORD)disp;
	}
}

//-----------------------------------------------------------