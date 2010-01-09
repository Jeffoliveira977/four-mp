/*
*	Created:			26.10.09
*	Created by:			009 & VoLT
*	Last Modifed:		-
*/

/* ----------------------------------------------------------------------------------------------------- */
/*                                Функции работы с игровыми потоками                                     */
/* ----------------------------------------------------------------------------------------------------- */
//DWORD (__cdecl * HashFromString)(char*) = (DWORD(__cdecl *)(char*))(0x16257C0 + dwLoadOffset);
DWORD dwLoadOffset = ((DWORD)GetModuleHandle (NULL)-0x400000);
/* ----------------------------------------------------------------------------------------------------- */
/*                                            Адреса игры                                                */
/* ----------------------------------------------------------------------------------------------------- */
#define ADDRESS_HASH_GET (0x6CF530+dwLoadOffset)
#define ADDRESS_THREAD_ID (0x17DF36C+dwLoadOffset)
#define ADDRESS_SCRIPTS_COUNT (0x17DF364+dwLoadOffset)
/* Threads */
#define ADDRESS_ACTIVE_THREAD (0x17DF370+dwLoadOffset)
#define ADDRESS_THREADS_ARRAY (0x18DF6C8+dwLoadOffset)
#define ADDRESS_THREAD_TICK (0xB4E1F0+dwLoadOffset)
/* Pools */
#define ADDRESS_PED_POOL (0x175B77C+dwLoadOffset)
#define ADDRESS_PLAYER_POOL (0x1033058+dwLoadOffset)
#define ADDRESS_VEHICLE_POOL (0x11F4F30+dwLoadOffset)
/* Offsets */
#define OFFSET_PLAYER_ID 0x4CA
#define OFFSET_PLAYER_MAX_HEALTH 0x53A
#define OFFSET_PLAYER_MAX_ARMOUR 0x53C
#define OFFSET_PLAYER_COLOR 0x560
#define OFFSET_PLAYER_PED 0x578


/* ----------------------------------------------------------------------------------------------------- */
/*                                            Функции хука                                               */
/* ----------------------------------------------------------------------------------------------------- */
// Получить массив потоков...
sysArray<GtaThread>* GetThreadsArray()
{
	return reinterpret_cast< sysArray<GtaThread>* >(ADDRESS_THREADS_ARRAY);
}

// Получить активный поток...
scrThread* GetActiveThread()
{
	// на выходе должна быть структура
	return *reinterpret_cast< scrThread** >(ADDRESS_ACTIVE_THREAD);
}

// Сменить активный поток...
void SetActiveThread(scrThread* thread)
{
	*reinterpret_cast< scrThread** >(ADDRESS_ACTIVE_THREAD) = thread;
}


/* ----------------------------------------------------------------------------------------------------- */
/*                                             Класс хука                                                */
/* ----------------------------------------------------------------------------------------------------- */

void FMPThread::AttachGtaThread()
{
	debug("FMPThread::AttachGtaThread called");

	while(*(PDWORD)ADDRESS_ACTIVE_THREAD == 0) Sleep(1);

	sysArray<GtaThread>* nowThreads = GetThreadsArray();

	int i;

	for(i = 0;i < nowThreads->wCount;i++)
	{
		/* Проверяем не занят ли слот потока */
		if(nowThreads->pData[i]->RetContext()->nThreadId == 0) break;
	}
	if(i == nowThreads->wCount) 
	{
		return;
	}

	m_pOriginalThread = nowThreads->pData[i];
	m_nThreadIndex = i;

	// Цепляем наш GtaThread в этот слот

	nowThreads->pData[i] = this;

	// получаем хеш названия GtaThread

	unsigned int hash;
	char* string_asm = "FMP";
	ptr call_this = ptr_cast(ADDRESS_HASH_GET);
	_asm
	{
		mov eax, string_asm;
		call call_this;
		mov hash, eax;
	}

	// Ставим чистую инфу в GtaThread

	reset(hash,NULL,0);

	// Забираем ид потока..
	unsigned int value;
	value = *(PDWORD)ADDRESS_THREAD_ID;
	*(PDWORD)ADDRESS_THREAD_ID = value + 1;

	// Говорим игре что у нас есть ещё 1 скрипт 


	DWORD ScriptsCount;
	ScriptsCount = *(PDWORD)ADDRESS_SCRIPTS_COUNT;

	*(PDWORD)ADDRESS_SCRIPTS_COUNT = ScriptsCount + 1;

	m_context.nThreadId = value;
	debug("FMPThread::AttachGtaThread complete");
}

ThreadStates FMPThread::reset(unsigned int hash,int v2,int i3)
{
	debug("FMPThread::reset called");
	m_context.dwOpcodeOff = 0;
	m_context.field_10 = 0;
	m_context.nStackOff = 0;
	m_context.nTimerA = 0;
	m_context.nTimerB = 0;
	m_context.nTimerC = 0;
	m_context.field_44 = 0;
	m_context.field_48 = 0;
	m_context.field_4C = 0;
	m_context.field_50 = 0;
	m_context.eThreadState = ThreadStateIdle;
	m_context.dwScriptHash = hash;
	m_pszExitMessage = "Normal exit";
	m_bCanBePaused = true;
	debug("FMPThread::reset complete");
	return m_context.eThreadState;
}

ThreadStates FMPThread::run(int i1)
{
	debug("FMPThread::run called");

	scrThread* oldThread = GetActiveThread();
	SetActiveThread(this);

	if(m_context.eThreadState != ThreadStateKilled) 
	{

	}

	SetActiveThread(oldThread);
	debug("FMPThread::run complete");
	return m_context.eThreadState;
}

ThreadStates FMPThread::tick(unsigned int msec)
{
	debug("FMPThread::tick called");

	ptr fn = ptr_cast(ADDRESS_THREAD_TICK);
	scrThread *thread = this;
	_asm
	{
		push msec;
		mov ecx, thread;
		call fn;
	}

	debug("FMPThread::tick complete");
	return m_context.eThreadState;
}


FMPThread::FMPThread()
{
	debug("FMPThread::FMPThread called");
	debug("FMPThread::FMPThread complete");
}

FMPThread::~FMPThread()
{
	debug("FMPThread::~FMPThread called");
	debug("FMPThread::~FMPThread complete");
}

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
