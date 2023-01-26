#pragma once
#include "debug.h"
#include "cusercmd.h"

typedef int(__stdcall* tCreateMove)(float sampletime, void* cmd);
tCreateMove oCreateMove;

int __stdcall hkCreateMove(float flsample, CUserCmd* cmd)
{
	int ret = oCreateMove(flsample, cmd);
	if (!cmd->command_number)
		return ret;

	g_pDebug->Print("stuff -> %d\n", cmd->tick_count);
	return ret;
}