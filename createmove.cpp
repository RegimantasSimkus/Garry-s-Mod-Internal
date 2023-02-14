#include "createmove.h"
#include "debug.h"
#include "c_gmod_player.h"

tCreateMove oCreateMove = nullptr;
int __stdcall hkCreateMove(float flsample, CUserCmd* cmd)
{
	int ret = oCreateMove(flsample, cmd);
	if (!cmd->command_number)
		return ret;

	C_GMOD_Player* localplayer = GetLocalPlayer();
	if (!localplayer)
		return ret;

	return ret;
}