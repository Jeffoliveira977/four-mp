#pragma once
/* ----------------------------------------------------------------------------------------------------- */
/*                                Функции работы с игровыми потоками                                     */
/* ----------------------------------------------------------------------------------------------------- */
//DWORD (__cdecl * HashFromString)(char*) = (DWORD(__cdecl *)(char*))(0x16257C0 + dwLoadOffset);
extern DWORD dwLoadOffset;
/* ----------------------------------------------------------------------------------------------------- */
/*                                            Адреса игры                                                */
/* ----------------------------------------------------------------------------------------------------- */
/* DirectX */
#define ADDRESS_CREATE_DEVICE (0xD301E4+dwLoadOffset)
/* Scripts */
#define ADDRESS_HASH_GET (0x5A7FC0+dwLoadOffset) 
#define ADDRESS_THREAD_ID (0x1848B00+dwLoadOffset)
#define ADDRESS_SCRIPTS_COUNT (0x1848B1C+dwLoadOffset)
/* Threads */
#define ADDRESS_ACTIVE_THREAD (0x1848B04+dwLoadOffset) 
#define ADDRESS_THREADS_ARRAY (0x1982320+dwLoadOffset)
#define ADDRESS_THREAD_TICK (0xBBCC40+dwLoadOffset)
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
/* Game */
#define CGAME_PROCESS_SLEEP (0x402B5C + dwLoadOffset)
#define CGAME_PROCESS_LOAD_MENU (0x402BEA + dwLoadOffset)
#define CGAME_PROCESS_START_GAME (0x402C1D + dwLoadOffset)

#define GAME_NAME (0xE9D13C+dwLoadOffset)

sysArray<GtaThread>* GetThreadsArray();
scrThread* GetActiveThread();
void SetActiveThread(scrThread* thread);

void JmpHook(DWORD from, DWORD to);
void CallHook(DWORD from, DWORD to);
void DataHook(DWORD from, DWORD to);
void PushHook(DWORD from, DWORD to);
void ChangeByte(DWORD from, BYTE b, int start = 0);
void SetString(DWORD address, char* string);

void InstallMethodHook(DWORD dwInstallAddress,DWORD dwHookFunction);
void InstallHook(DWORD dwInstallAddress,DWORD dwHookFunction,DWORD dwHookStorage,BYTE* pbyteJmpCode,int iJmpCodeSize);

bool Unlock(void *address, int len);
void Call(DWORD from, DWORD to);

#define POINTER_TO_MEMBER(m,n,o) temp = n, m = *(o *)&temp