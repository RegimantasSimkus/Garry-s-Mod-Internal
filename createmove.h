#pragma once
#include "cusercmd.h"

typedef int(__stdcall* tCreateMove)(float sampletime, void* cmd);
extern tCreateMove oCreateMove;

int __stdcall hkCreateMove(float flsample, CUserCmd* cmd);