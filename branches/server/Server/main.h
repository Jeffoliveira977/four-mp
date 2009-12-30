/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#ifndef MAX_PLAYERS
#include "struct.h"

// LIMITS
#define MAX_PLAYERS 32
#define MAX_VEHICLES 64
#define MAX_PCLASS 64

// VARS
extern Player gPlayer[MAX_PLAYERS];
extern Vehicle gVehicle[MAX_VEHICLES];
extern Server sConf;
extern PlayerClass pClass[MAX_PCLASS];

void print(const char *string, ...);
void debug(const char *string, ...);
#endif