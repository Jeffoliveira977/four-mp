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
