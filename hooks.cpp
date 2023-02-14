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
	CreateMove = ClientModeShared->Hook(21, (PVOID)hkCreateMove, reinterpret_cast<PVOID*>(&oCreateMove));

#ifndef _WIN64
	pD3DDevice = new VMTHookManager(
	(PVOID)(**(DWORD**)(FindSignature("shaderapidx9.dll", "\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\x53\x56\x57", "xxxxx????xxx") + 0x116 + 2))
// #else
		FindSignature("shaderapidx9.dll", "\x8B\xCF\x89\x35\x00\x00\x00\x00", "xxxx????")
	);
#else
	{
		uintptr_t mov = FindSignature("shaderapidx9.dll", "\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxx????xxx????") + 0x2B8 + 3;
		pD3DDevice = new VMTHookManager(
			(PVOID)(
				*(uintptr_t*)(mov + *(int*)mov + 4)
			)
		);
	}

	EndScene = pD3DDevice->Hook(42, (PVOID)hkEndScene, reinterpret_cast<PVOID*>(&oEndScene));
#endif

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