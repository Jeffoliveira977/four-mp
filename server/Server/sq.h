/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "sq/squirrel.h"
#include "sq/sqstdsystem.h"

// SQ Script Functions
void sq_CreateCar(HSQUIRRELVM v);
void sq_GiveWeapon(HSQUIRRELVM v);
void sq_addPlayerClass(HSQUIRRELVM v);
void sq_enableComponentSelect(HSQUIRRELVM v);

void sq_SetGameTime(HSQUIRRELVM v);
void sq_GetGameHour(HSQUIRRELVM v);
void sq_GetGameMinutes(HSQUIRRELVM v);