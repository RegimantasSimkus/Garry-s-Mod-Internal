#include "hooks.h"
#include "Interfaces.h"
#include "createmove.h"

CHooks* Hooks = nullptr;
CHooks::CHooks()
{
	// ClientModeShared
	ClientModeShared = new VMTHookManager(Interface->ClientModeShared);
	CreateMove = ClientModeShared->Hook(21, (PVOID)hkCreateMove, reinterpret_cast<PVOID*>(&oCreateMove));

	// and so on
}

void CHooks::Release()
{
	// unhook everything
	CreateMove->Restore();

	// clean up memory
	delete ClientModeShared;
	delete CreateMove;
	delete this;
}