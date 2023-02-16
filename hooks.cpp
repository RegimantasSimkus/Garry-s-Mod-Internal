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
		(PVOID)(**(DWORD**)(FindSignature("shaderapidx9.dll", "\x89\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x80\xA7\x00\x00\x00\x00\x00", "xx????x????xx?????") + 2))
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

#endif

	EndScene = new CTrampHook(pD3DDevice->vTable[42], (PVOID)hkEndScene, 7, (PVOID*)&oEndScene);
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