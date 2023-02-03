#include "hooks.h"
#include "Interfaces.h"
#include "createmove.h"
#include "sigscan.h"
#include "endscene.h"

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

	pD3DDevice = new VMTHookManager(
		(PVOID)(**(DWORD**)(FindSignature("shaderapidx9.dll", "\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\x53\x56\x57", "xxxxx????xxx") + 0x116 + 2))
	);

	EndScene = pD3DDevice->Hook(42, (PVOID)hkEndScene, reinterpret_cast<PVOID*>(&oEndScene));

	// and so on
}

#define SAFE_RESTORE(name) \
	if (this->name) \
	{	\
		this->name->Restore(); \
		delete this->name; \
	}

void CHooks::Release()
{
	// unhook everything
	SAFE_RESTORE(CreateMove);
	SAFE_RESTORE(EndScene);

	// clean up memory
	if (ClientModeShared)
		delete ClientModeShared;

	delete this;
}