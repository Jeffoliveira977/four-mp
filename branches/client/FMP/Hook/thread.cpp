#include "classes.h"
#include "hook.h"
#include "../log.h"

DWORD dwLoadOffset = ((DWORD)GetModuleHandle (NULL)-0x400000);

// �������� ������ �������...
sysArray<GtaThread>* GetThreadsArray()
{
	return reinterpret_cast< sysArray<GtaThread>* >(ADDRESS_THREADS_ARRAY);
}

// �������� �������� �����...
scrThread* GetActiveThread()
{
	// �� ������ ������ ���� ���������
	return *reinterpret_cast< scrThread** >(ADDRESS_ACTIVE_THREAD);
}

// ������� �������� �����...
void SetActiveThread(scrThread* thread)
{
	*reinterpret_cast< scrThread** >(ADDRESS_ACTIVE_THREAD) = thread;
}

void FMPThread::AttachGtaThread()
{
	debug("FMPThread::AttachGtaThread called");

	while(*(PDWORD)ADDRESS_ACTIVE_THREAD == 0) Sleep(1);

	sysArray<GtaThread>* nowThreads = GetThreadsArray();

	int i;

	for(i = 0;i < nowThreads->wCount;i++)
	{
		/* ��������� �� ����� �� ���� ������ */
		if(nowThreads->pData[i]->RetContext()->nThreadId == 0) break;
	}
	if(i == nowThreads->wCount) 
	{
		return;
	}

	m_pOriginalThread = nowThreads->pData[i];
	m_nThreadIndex = i;

	// ������� ��� GtaThread � ���� ����

	nowThreads->pData[i] = this;

	// �������� ��� �������� GtaThread

	unsigned int hash;
	char* string_asm = "FMP";
	ptr call_this = ptr_cast(ADDRESS_HASH_GET);
	_asm
	{
		mov eax, string_asm;
		call call_this;
		mov hash, eax;
	}

	// ������ ������ ���� � GtaThread

	reset(hash,NULL,0);

	// �������� �� ������..
	unsigned int value;
	value = *(PDWORD)ADDRESS_THREAD_ID;
	*(PDWORD)ADDRESS_THREAD_ID = value + 1;

	// ������� ���� ��� � ��� ���� ��� 1 ������ 


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