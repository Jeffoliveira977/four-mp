#include "classes.h"
#include "hook.h"
#include "../log.h"


FMPHook::FMPHook(): FMPThread()
{
	m_pPrimaryFiber = NULL;
	m_pScriptFiber = NULL;
	m_bKillRequested = false;
	m_nWaitTick = 0;
}

FMPHook::~FMPHook()
{
	debug("FMPHook::~FMPHook called");
	debug("FMPHook::~FMPHook complete");
}

ThreadStates FMPHook::Reset(unsigned int hash,int v2,int i3)
{
	debug("FMPHook::reset called");
	if (m_pScriptFiber)
		DeleteFiber(m_pScriptFiber);

	m_pScriptFiber = CreateFiber(0, &FiberStart, this);

	if (!m_pScriptFiber)
	{
		Kill();
		return m_context.eThreadState;
	}
	debug("FMPHook::reset complete");
	return FMPThread::Reset(hash,v2,i3);
}

void FMPHook::FiberStart(void* parameter)
{
	debug("FMPHook::FiberStart called");
	FMPHook* Thread = ptr_cast<FMPHook>(parameter);
	Thread->GameThread();
	Thread->m_bKillRequested = true;
	SwitchToFiber(Thread->m_pPrimaryFiber);
	debug("FMPHook::FiberStart complete");
}

ThreadStates FMPHook::Run(int i1)
{
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

		if (!m_pScriptFiber)
		{
			Kill();
			return m_context.eThreadState;
		}
	}

	scrThread* old = GetActiveThread();
	SetActiveThread(this);

	if(m_context.eThreadState != ThreadStateKilled)
		if(GetTickCount() > m_nWaitTick) SwitchToFiber(m_pScriptFiber);

	if(m_bKillRequested) Kill();

	SetActiveThread(old);
	return m_context.eThreadState;
}

void FMPHook::wait(unsigned int timeMS)
{
	m_nWaitTick = GetTickCount() + timeMS;
	SwitchToFiber(m_pPrimaryFiber);
}

void FMPHook::Kill()
{
	debug("FMPHook::Kill called");
	if (GetCurrentFiber() != m_pPrimaryFiber) return;

	DeleteFiber(m_pScriptFiber);

	m_pPrimaryFiber = NULL;
	m_pScriptFiber = NULL;
	m_bKillRequested = false;

	FMPThread::Kill();
	debug("FMPHook::Kill complete");
}

bool FMPHook::IsThreadAlive()
{
	return m_context.eThreadState != ThreadStateKilled;
}

void FMPHook::TerminateThisScript()
{
	debug("FMPHook::TerminateThisScript called");
	if (GetCurrentFiber() != m_pScriptFiber)
		return;

	m_bKillRequested = true;	
	SwitchToFiber(m_pPrimaryFiber);
	debug("FMPHook::TerminateThisScript complete");
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

void JmpHook(DWORD from, DWORD to) 
{
    DWORD oldp;
    VirtualProtect((LPVOID)from, 5, PAGE_EXECUTE_READWRITE, &oldp);
    BYTE *patch = (BYTE *)from;
    *patch = 0xE9;    // JMP
    *(DWORD *)(patch + 1) = (to - (from + 5));    
}

void CallHook(DWORD from, DWORD to) 
{
    DWORD oldp;
    VirtualProtect((LPVOID)from, 5, PAGE_EXECUTE_READWRITE, &oldp);
    BYTE *patch = (BYTE *)from;
    *patch = 0xFF;    // CALL
    *(DWORD *)(patch + 1) = (to - (from + 5));    
}

void PushHook(DWORD from, DWORD to) 
{
    DWORD oldp;
    VirtualProtect((LPVOID)from, 5, PAGE_EXECUTE_READWRITE, &oldp);
    BYTE *patch = (BYTE *)from;
    *patch = 0x68;    // PUSH
    *(DWORD *)(patch + 1) = (to - (from + 5));    
}

void DataHook(DWORD from, DWORD to) 
{
    DWORD oldp;
    VirtualProtect((LPVOID)from, 5, PAGE_EXECUTE_READWRITE, &oldp);
    BYTE *patch = (BYTE *)from;
    *(DWORD *)(patch + 1) = to;   
}

void ChangeByte(DWORD from, BYTE b, int start)
{
	BYTE *patch = (BYTE *)from;
	*(BYTE*)(patch+start) = b;
}

//-----------------------------------------------------------

void SetString(DWORD address, char* string) 
{
    DWORD oldp;
    VirtualProtect((PVOID)address,2,PAGE_EXECUTE_READWRITE,&oldp);
    strcpy((PCHAR)address,string);
}