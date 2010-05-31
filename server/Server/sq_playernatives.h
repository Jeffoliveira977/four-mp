#pragma once

#include "sq/squirrel.h"

void sq_GetPlayerName(HSQUIRRELVM v);
void sq_GetPlayerModel(HSQUIRRELVM v);
void sq_GetPlayerAngle(HSQUIRRELVM v);
void sq_GetPlayerPosition(HSQUIRRELVM v);
void sq_GetPlayerVehicle(HSQUIRRELVM v);
void sq_GetPlayerScore(HSQUIRRELVM v);
void sq_GetPlayerHealth(HSQUIRRELVM v);
void sq_GetPlayerArmor(HSQUIRRELVM v);
void sq_GetPlayerWantedLevel(HSQUIRRELVM v);

void sq_SetPlayerSpawnPos(HSQUIRRELVM v);
void sq_SetPlayerModel(HSQUIRRELVM v);