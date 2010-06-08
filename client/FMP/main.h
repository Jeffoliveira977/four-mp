#pragma once 
#include "structs.h"
// --------------------------------------------------
// defines
// --------------------------------------------------
#define MAX_PLAYERS 32
#define MAX_CARS 64
#define MAX_PCLASS 64

#ifndef main_cpp

extern FPlayer gPlayer[MAX_PLAYERS];
extern FVehicle gCar[MAX_CARS];

extern bool myEnter;

#endif