#pragma once
#include "vmthook.h"

class CHooks
{
public:
	VMTHookManager* ClientModeShared;
	VMTHook* CreateMove;
	void Release();

	CHooks();
};

extern CHooks* Hooks;