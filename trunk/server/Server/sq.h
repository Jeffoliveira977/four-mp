/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "sq/squirrel.h"
#include "sq/sqstdsystem.h"

// SQ Script Functions
void sq_CreateCar(HSQUIRRELVM v);
void sq_addPlayerClass(HSQUIRRELVM v);
void sq_enableComponentSelect(HSQUIRRELVM v);

void sq_GetGameTime(HSQUIRRELVM v);
void sq_SetGameTime(HSQUIRRELVM v);

void sq_SetCmdCharFirst(HSQUIRRELVM v);
void sq_SetCmdCharSecond(HSQUIRRELVM v);

void sq_SendMessageToAll(HSQUIRRELVM v);
void sq_SendMessageToPlayer(HSQUIRRELVM v);

void sq_Kick(HSQUIRRELVM v);
void sq_Ban(HSQUIRRELVM v);
void sq_ReloadBanList(HSQUIRRELVM v);
void sq_ClearBanList(HSQUIRRELVM v);

void sq_include(HSQUIRRELVM v);