#pragma once
#include "vmthook.h"

class CHooks
{
public:
	VMTHookManager* ClientModeShared;
	VMTHook* CreateMove;
	VMTHookManager* pD3DDevice;
	VMTHook* EndScene;
	void Release();

	CHooks();
};

extern CHooks* Hooks;