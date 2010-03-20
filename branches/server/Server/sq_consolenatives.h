#pragma once

#include "sq\squirrel.h"

void sq_PrintToServer(HSQUIRRELVM v, const SQChar *s, ...);
void sq_printr(HSQUIRRELVM v);
void sq_CreateConVar(HSQUIRRELVM v);
void sq_FindConVar(HSQUIRRELVM v);
void sq_ResetConVar(HSQUIRRELVM v);
void sq_GetConVarName(HSQUIRRELVM v);
void sq_GetConVarFloat(HSQUIRRELVM v);
void sq_GetConVarInt(HSQUIRRELVM v);
void sq_GetConVarString(HSQUIRRELVM v);
void sq_GetConVarFlags(HSQUIRRELVM v);
void sq_GetConVarBoundFloat(HSQUIRRELVM v);
void sq_GetConVarBoundInt(HSQUIRRELVM v);
void sq_SetConVarFloat(HSQUIRRELVM v);
void sq_SetConVarInt(HSQUIRRELVM v);
void sq_SetConVarString(HSQUIRRELVM v);
void sq_SetConVarFlags(HSQUIRRELVM v);
void sq_SetConVarBoundFloat(HSQUIRRELVM v);
void sq_SetConVarBoundInt(HSQUIRRELVM v);
void sq_RegServerCmd(HSQUIRRELVM v);
void sq_GetCmdArgs(HSQUIRRELVM v);
void sq_GetCmdArgsAsString(HSQUIRRELVM v);
void sq_GetCmdArgType(HSQUIRRELVM v);
void sq_GetCmdArgString(HSQUIRRELVM v);
void sq_GetCmdArgInt(HSQUIRRELVM v);
void sq_GetCmdArgFloat(HSQUIRRELVM v);
void sq_ServerCommand(HSQUIRRELVM v);