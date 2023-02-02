#include "hooks.h"
#include "Interfaces.h"
#include "createmove.h"

CHooks* Hooks = nullptr;
CHooks::CHooks()
{
	// ClientModeShared
	if (!Interface->ClientModeShared)
	{
		g_pDebug->Print("Failed to initialize hooks :(\n");
		return;
	}
	ClientModeShared = new VMTHookManager(Interface->ClientModeShared);
	// CreateMove = ClientModeShared->Hook(21, (PVOID)hkCreateMove, reinterpret_cast<PVOID*>(&oCreateMove));

	// and so on
}

void CHooks::Release()
{
	// unhook everything
	if (CreateMove)
	{
		CreateMove->Restore();
		delete CreateMove;
	}

	// clean up memory
	if (ClientModeShared)
		delete ClientModeShared;

	delete this;
}