#pragma once
#include "vmthook.h"
#include "tramphook.h"

class CHooks
{
public:
	VMTHookManager* ClientModeShared;
	VMTHook* CreateMove;
	VMTHookManager* pD3DDevice;
	CTrampHook* EndScene;
	void Release();

	CHooks();
};

extern CHooks* Hooks;